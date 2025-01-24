#include "DeviceGraph.hpp"

#include <Process/Dataflow/Cable.hpp>
#include <Process/Dataflow/Port.hpp>
#include <Process/Process.hpp>

#include <score/document/DocumentContext.hpp>

#include <ossia/detail/algorithms.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/topological_sort.hpp>

#include <iostream>

namespace Pico
{

template <typename Graph_T, typename IO>
void print_graph(Graph_T& g, IO& stream, auto& components)
{
  std::stringstream s;
  boost::write_graphviz(
      s,
      g,
      [&](auto& out, auto v)
      {
        if (Process::ProcessModel* n = g[v])
        {
          out << "[label=\"" << components[v] << " => "
              << n->prettyName().toStdString() << "\"]";
        }
        else
          out << "[]";
      },
      [](auto&&...) {});

  stream << s.str() << "\n";
}

using ProcessGraph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    Process::ProcessModel*,
    boost::no_property>;
using UDProcessGraph = boost::adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::undirectedS,
    Process::ProcessModel*,
    boost::no_property>;

Graph processGraph(
    const score::DocumentContext& context,
    const std::vector<Process::ProcessModel*>& processes)
{
  const int N = processes.size();
  // make a graph and use boost::connected_components to generate tasks
  ProcessGraph graph;
  UDProcessGraph udgraph;
  std::vector<unsigned long> processes_vtx;

  // 1. Create the graph
  for (auto& proc : processes)
  {
    processes_vtx.push_back(boost::add_vertex(proc, graph));
    boost::add_vertex(proc, udgraph);
  }

  int vtx_index_outlet = 0;
  for (Process::ProcessModel* proc : processes)
  {
    for (auto& outl : proc->outlets())
    {
      for (auto& cable : outl->cables())
      {
        auto& c = cable.find(context);
        auto& inlt = c.sink().find(context);

        Process::ProcessModel* inlt_proc = safe_cast<Process::ProcessModel*>(
            inlt.parent()); // FIXME this does not handle sub-inlets
        if (auto it = ossia::find(processes, inlt_proc); it != processes.end())
        {
          int vtx_index_inlet = std::distance(processes.begin(), it);

          boost::add_edge(
              processes_vtx[vtx_index_outlet],
              processes_vtx[vtx_index_inlet],
              graph);
          boost::add_edge(
              processes_vtx[vtx_index_outlet],
              processes_vtx[vtx_index_inlet],
              udgraph);
        }
      }
    }

    vtx_index_outlet++;
  }

  // 2. Compute the connected components
  std::vector<unsigned int> component_map;
  component_map.resize(N);

  int component_count
      = boost::connected_components(udgraph, component_map.data());

  // 3. Print graph
  print_graph(graph, std::cerr, component_map);

  // 4. Toposort
  ossia::pod_vector<unsigned long> topo;
  topo.reserve(N);
  boost::topological_sort(graph, std::back_inserter(topo));

  // 5. Make topologically sorted subcombonents
  // Note: boost toposort gives a reverse order sort which is generally what we want
  // (e.g. for "pull" algorithms: the last node in a chain comes first).
  // Here we are in push mode so we want the "normal" topo sort, e.g. the reverse
  // of boost's
  Graph g;
  g.tasks.resize(component_count);
  g.topo_order.reserve(N);
  GraphTasks& components = g.tasks;

  for (int i = 0; i < component_count; i++)
    components[i].taskName = QString::number(i);
  for (auto it = topo.rbegin(); it != topo.rend(); ++it)
  {
    unsigned long vtx = *it;
    Process::ProcessModel* process = graph[vtx];
    int component = component_map.at(vtx);

    components.at(component).processes.push_back(process);

    g.topo_order.push_back(process);
  }

  // 6. Store the used addresses

  for(auto& task : components)
  {
    for(auto& proc : task.processes)
    {
      for(auto inl : proc->inlets())
        if(const auto& addr = inl->address(); addr.isSet())
        {
          g.in_addresses[addr.address.device].insert(addr.address.path);
          g.merged_addresses[addr.address.device].insert(addr.address.path);
        }
      for(auto outl : proc->outlets())
        if(const auto& addr = outl->address(); addr.isSet())
        {
          g.out_addresses[addr.address.device].insert(addr.address.path);
          g.merged_addresses[addr.address.device].insert(addr.address.path);
        }
    }
  }
  return g;
}

}
