#include "DeviceGraph.hpp"

#include <Process/Dataflow/Cable.hpp>
#include <Process/Dataflow/Port.hpp>
#include <Process/ProcessList.hpp>

#include <score/document/DocumentContext.hpp>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/connected_components.hpp>
#include <boost/graph/graphviz.hpp>
#include <boost/graph/topological_sort.hpp>

#include <avnd/introspection/input.hpp>
#include <avnd/introspection/output.hpp>

#include <avnd/../../examples/Advanced/Utilities/Math.hpp>
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

struct world
{
  template <std::size_t N>
  float read_local();
  template <std::size_t N>
  void write_local(float v);
  template <std::size_t N>
  float read_remote();
  template <std::size_t N>
  void write_remote(float v);
};

QString Device::printTask(
    const score::DocumentContext& context,
    const std::vector<Process::ProcessModel*>& procs)
{
  std::string c;
  c.reserve(2000);
  c += "{\n";

  std::vector<std::unique_ptr<Process::CodeWriter>> codes;

  for (auto* p : procs)
  {
    qDebug() << p->metadata().getName() << p->prettyName();
    auto writer = p->codeWriter(Process::CodeFormat::Cpp);
    if (writer)
    {
      codes.push_back(std::move(writer));
    }
  }

  // 1. Instantiate the processes
  {
    int index = 0;
    for (auto& wr : codes)
    {
      wr->variable = fmt::format("p{}", index);
      c += fmt::format("{} {};\n", wr->typeName(), wr->variable);
      index++;
    }
  }

  auto address_to_device_read_index
      = [](const State::Address& addr) { return 0; };
  auto address_to_network_read_index
      = [](const State::Address& addr) { return 0; };
  auto address_to_device_write_index
      = [](const State::Address& addr) { return 0; };
  auto address_to_network_write_index
      = [](const State::Address& addr) { return 0; };

  // Start the loop
  c += "for(;;) { \n";

  // 2. Write the inputs addresses
  {
    int process_index = 0;
    for (auto* p : procs)
    {
      auto& wr = codes[process_index];

      for (auto inl : p->inlets())
      {
        if (inl->address().address.device == this->name)
        {
          c += fmt::format(
              "{{ {} = world.read_local<{}>(); }}\n",
              wr->accessInlet(inl->id()),
              address_to_device_read_index(inl->address().address));
        }
        else if (!inl->address().address.device.isEmpty())
        {
          c += fmt::format(
              "{{ {} = world.read_network<{}>(); }}\n",
              wr->accessInlet(inl->id()),
              address_to_network_read_index(inl->address().address));
        }
      }

      process_index++;
    }
  }

  // 3. Write down the graph
  {
    int process_index = 0;
    for (auto& wr : codes)
    {
      auto& p = *procs[process_index];
      // Set all the cable inlets
      for (auto inl : p.inlets())
      {
        for (auto cable : inl->cables())
        {
          Process::Outlet* out = &cable.find(context).source().find(context);
          auto* parent = safe_cast<Process::ProcessModel*>(out->parent());
          if (int index_of_parent = ossia::index_in_container(procs, parent);
              index_of_parent != -1)
          {
            c += fmt::format(
                "{{ {} = {}; }}\n",
                wr->accessInlet(inl->id()),
                codes[index_of_parent]->accessOutlet(out->id()));
          }
          else
          {
            // FIXME
            SCORE_TODO;
          }
        }
      }

      // run
      c += wr->execute() + "\n";

      process_index++;
    }
  }

  // 4. Write the outputsaddresses
  {
    int process_index = 0;
    for (auto* p : procs)
    {
      auto& wr = codes[process_index];

      for (auto inl : p->outlets())
      {
        if (inl->address().address.device == this->name)
        {
          c += fmt::format(
              "{{ world.write_local<{}>({}); }}\n",
              address_to_device_write_index(inl->address().address),
              wr->accessOutlet(inl->id()));
        }
        else if (!inl->address().address.device.isEmpty())
        {
          c += fmt::format(
              "{{ world.write_network<{}>({}); }}\n",
              address_to_network_write_index(inl->address().address),
              wr->accessOutlet(inl->id()));
        }
      }

      process_index++;
    }
  }

  // End the loop
  c += "}\n}\n";

  return QString::fromStdString(c);
}

void Device::processGraph(const score::DocumentContext& context)
{
  const int N = processes.size();
  // make a graph and use boost::connected_components to generate tasks
  ProcessGraph graph;
  std::vector<unsigned long> processes_vtx;

  // 1. Create the graph
  for (auto& proc : processes)
  {
    processes_vtx.push_back(boost::add_vertex(proc, graph));
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
        }
      }
    }

    vtx_index_outlet++;
  }

  // 2. Compute the connected components
  std::vector<unsigned int> component_map;
  component_map.resize(N);

  int component_count
      = boost::connected_components(graph, component_map.data());

  // 3. Print graph
  print_graph(graph, std::cerr, component_map);

  // 4. Toposort
  ossia::pod_vector<unsigned long> topo;
  topo.reserve(N);
  boost::topological_sort(graph, std::back_inserter(topo));

  // 4. Make topologically sorted subcombonents
  // Note: boost toposort gives a reverse order sort which is generally what we want
  // (e.g. for "pull" algorithms: the last node in a chain comes first).
  // Here we are in push mode so we want the "normal" topo sort, e.g. the reverse
  // of boost's
  std::vector<std::vector<Process::ProcessModel*>> components(component_count);

  for (auto it = topo.rbegin(); it != topo.rend(); ++it)
  {
    unsigned long vtx = *it;
    Process::ProcessModel* process = graph[vtx];
    int component = component_map.at(vtx);

    qDebug() << "toposort: " << component << process->prettyName();
    components.at(component).push_back(process);
  }

  // 4. Print source code
  QString source;
  source += R"_(
#include <cstdio>
#include <cmath>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <avnd/common/export.hpp>
#include <avnd/introspection/input.hpp>
#include <avnd/introspection/messages.hpp>
#include <avnd/introspection/output.hpp>
#include <avnd/introspection/port.hpp>
#include <avnd/wrappers/avnd.hpp>
#include <avnd/wrappers/controls.hpp>
#include <avnd/wrappers/metadatas.hpp>
#include <avnd/../../examples/Advanced/Utilities/Math.hpp>
#include <ExprtkMapper.hpp>


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <driver/gpio.h>
#include <driver/ledc.h>

#include <esp_err.h>

)_";

  std::string task_creation;
  int task_n = 0;
  for (auto& comp : components)
  {
    source += "\n";
    source += fmt::format("static void ossia_task_{}(void* arg) {{\n", task_n);
    source += printTask(context, comp);
    source += "}\n";

    task_creation += fmt::format(
        R"_(xTaskCreate(ossia_task_{0}, "ossia_task_{0}", 4096, nullptr, 10, nullptr);)_",
        task_n);
    task_creation += "\n";
    task_n++;
  }

  // Write the main:

  source += fmt::format(
      R"_(
extern "C" void app_main(void)
{{
   // 1. Initialize all the things we need from the device
  {{
    {}
  }}

  // 2. Create all the tasks
  {{
    {}
  }}

   // 3. lesgo
   for(;;) vTaskDelay(1000);
}}
)_",
      printDeviceInitialization().toStdString(),
      task_creation);

  qDebug() << source.toStdString().c_str();

  // - First list all the used processes
  // => for now we'll just include them all in the header

  // - Abstract the input reading / output writing
  // => see world.read_/write_

  // - Faust code? https://faustdoc.grame.fr/tutorials/esp32/
  // - ExprTK code? try to compile it to C directly,
  // - Reduction step for basic processes? e.g. addition, etc

#if 0
  // Let's assume we want to do gpio2 = sin(gpio0 + gpio1)
  float gpio0{}, gpio1{}, gpio2{};
  world w;
  avnd::blocks::Addition n0;
  avnd::blocks::Sin n1;

  // Addresses in:
  avnd::parameter_input_introspection<decltype(n0)>::get<0>(n0.inputs).value
      = w.read_local<0>();
  avnd::parameter_input_introspection<decltype(n0)>::get<1>(n0.inputs).value
      = w.read_local<1>();

  n0();

  avnd::parameter_input_introspection<decltype(n1)>::get<0>(n1.inputs).value
      = avnd::parameter_output_introspection<decltype(n0)>::get<0>(n0.outputs)
            .value;
  n1();

  // Addresses out
  w.write_local<2>(
      avnd::parameter_output_introspection<decltype(n1)>::get<0>(n1.outputs)
          .value);
#endif
}

QString Device::printDeviceInitialization()
{
  QString str;

  for (auto& ios : this->ios)
  {
    if (ios.direction == ios.Input)
    {
      str += "{ \n" + ios.printInitialization() + "\n }\n";
    }
  }
  for (auto& ios : this->ios)
  {
    if (ios.direction == ios.Output)
    {
      str += "{ \n" + ios.printInitialization() + "\n }\n";
    }
  }

  return str;
}

QString DeviceIO::printInitialization()
{
  switch (type)
  {
    case GPIO:
    {
      QString str;
      str = QString::fromStdString(fmt::format(
          R"_(
        gpio_config_t io_conf = {{}};
        io_conf.intr_type = GPIO_INTR_DISABLE;
        io_conf.mode = {};
        //bit mask of the pins that you want to set,e.g.GPIO18/19
        io_conf.pin_bit_mask = (1ULL << GPIO_NUM_{});
        io_conf.pull_down_en = gpio_pulldown_t::GPIO_PULLDOWN_DISABLE;
        io_conf.pull_up_en = gpio_pullup_t::GPIO_PULLUP_DISABLE;
        gpio_config(&io_conf);
      )_",
          this->direction == Input ? "GPIO_MODE_INPUT" : "GPIO_MODE_OUTPUT",
          this->pin));

      return str;
    }
    case PWM:
    {
      QString str;
      str = QString::fromStdString(fmt::format(
          R"_(

  // Prepare and then apply the LEDC PWM timer configuration
  ledc_timer_config_t ledc_timer = {{
      .speed_mode = LEDC_HIGH_SPEED_MODE,
      .duty_resolution = LEDC_TIMER_8_BIT,
      .timer_num = LEDC_TIMER_0,
      .freq_hz = 3000, // Set output frequency at 5 kHz
      .clk_cfg = LEDC_USE_REF_TICK}};
  ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

  // Prepare and then apply the LEDC PWM channel configuration
  ledc_channel_config_t ledc_channel = {{.gpio_num = GPIO_NUM_{},
                                        .speed_mode = LEDC_HIGH_SPEED_MODE,
                                        .channel = LEDC_CHANNEL_0,
                                        .intr_type = LEDC_INTR_DISABLE,
                                        .timer_sel = LEDC_TIMER_0,
                                        .duty = 0, // Set duty to 0%
                                        .hpoint = 0}};
  ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
)_",
          this->pin));

      return str;
    }
  }
}

}
