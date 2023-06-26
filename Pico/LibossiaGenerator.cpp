#include "LibossiaGenerator.hpp"

#include <Process/CodeWriter.hpp>
#include <Process/Dataflow/Cable.hpp>
#include <Process/Dataflow/Port.hpp>
#include <Process/Process.hpp>

#include <score/model/path/Path.hpp>

#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/fmt.hpp>
#include <ossia/detail/hash_map.hpp>

#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Document/Interval/IntervalModel.hpp>
#include <Scenario/Document/State/StateModel.hpp>
#include <Scenario/Document/TimeSync/TimeSyncModel.hpp>
#include <Scenario/Process/Algorithms/Accessors.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <fmt/format.h>
// clang-format off
namespace Pico
{
template <typename T>
static std::string id(const T& c)
{
  QString tmp = Path<T>(c).unsafePath().toString();
  tmp.remove("Scenario::ScenarioDocumentModel.1/Scenario::BaseScenario.0/");
  tmp.remove("Scenario::");
  tmp.replace("IntervalModel", "itv");
  tmp.replace("TimeSyncModel", "ts");
  tmp.replace("EventModel", "ev");
  tmp.replace("StateModel", "st");
  tmp.replace("Scenario", "sc");
  tmp.replace("Process::ProcessModel", "p");
  tmp.replace('/', '_');
  tmp.replace('.', '_');
  tmp.replace(':', '_');
  tmp.replace("__", "_");
  if(tmp.back() == '_')
    tmp.resize(tmp.size() - 1);
  return tmp.toStdString();
}


class CPPVisitor
{
  ossia::hash_map<const Scenario::IntervalModel*, int> itv_ids;
  ossia::hash_map<const Scenario::EventModel*, int> ev_ids;
  ossia::hash_map<const Scenario::TimeSyncModel*, int> trig_ids;
  ossia::hash_map<const Process::ProcessModel*, int> proc_ids;
  int cur_itv_id{};
  int cur_node_id{};
  int cur_proc_id{};
  int cur_cond_id{};
  int cur_trig_id{};
  int indent{};

  void finishList()
  {
    if (text.endsWith("; "))
      text.resize(text.size() - 2);
  }

  template<typename... Args>
  void addLine(fmt::format_string<Args...> str, Args&&... args) {
    text += QString(2 * indent, ' ') +
            QString::fromStdString(
                fmt::format(str, std::forward<Args>(args)...)
                ) + "\n";
  }

  void addLine(const char* str) {
    text += QString(2 * indent, ' ') + str + "\n";
  }

public:
  QString text;
  CPPVisitor() = default;

  void operator()(const Scenario::ProcessModel& proc)
  {
    int id = ++cur_proc_id;
    proc_ids.insert({&proc, id});
    addLine("auto {} = std::make_shared<ossia::scenario>();", Pico::id(proc));
    addLine("");

    for (auto& ts : proc.timeSyncs)
    {
      if(&ts == &proc.startTimeSync())
      {
        addLine("const auto& {} = {}->get_start_time_sync();", Pico::id(ts), Pico::id(proc));
      }
      else
      {
        addLine("auto {} = std::make_shared<ossia::time_sync>();", Pico::id(ts));
        addLine("{}->set_expression(ossia::expressions::make_expression_true());", Pico::id(ts));
        addLine("{}->add_time_sync({});", Pico::id(proc), Pico::id(ts));
      }
    }

    addLine("");
    for (auto& ev : proc.events)
    {
      if(&ev == &proc.startEvent())
      {
        addLine("const auto& {} = *{}->get_start_time_sync()->get_time_events().begin();", Pico::id(ev), Pico::id(proc));
      }
      else
      {
        const auto& parent_ts_id = Pico::id(Scenario::parentTimeSync(ev, proc));
        addLine("auto {} = std::make_shared<ossia::time_event>(ossia::time_event::exec_callback{{}}, "
            "*{}, "
            "ossia::expressions::make_expression_true());", Pico::id(ev), parent_ts_id);
        addLine("{}->insert({}->get_time_events().end(), {});", parent_ts_id, parent_ts_id, Pico::id(ev));
      }
    }

    addLine("");
    for (auto& itv : proc.intervals)
    {
      const auto& id = Pico::id(itv);
      addLine("auto {} = ossia::time_interval::create({{}}, *{}, *{}, {}_tv, {}_tv, {}_tv);",
              id,
              Pico::id(Scenario::startEvent(itv, proc)),
              Pico::id(Scenario::endEvent(itv, proc)),
              itv.duration.defaultDuration().impl,
              itv.duration.minDuration().impl,
              itv.duration.maxDuration().impl
              );
      if(itv.graphal()) {
        addLine("{}->graphal = true;", id);
      }
      addLine("{}->add_time_interval({});", Pico::id(proc), id);
    }
  }
};

std::string scenarioToCPP(const Scenario::ProcessModel& s)
{
  CPPVisitor m;
  m(s);
  return m.text.toStdString();
}
}



namespace Pico
{
class CPPVisitorStructHeader
{
public:
  ossia::hash_map<const Scenario::IntervalModel*, int> itv_ids;
  ossia::hash_map<const Scenario::EventModel*, int> ev_ids;
  ossia::hash_map<const Scenario::TimeSyncModel*, int> trig_ids;
  //ossia::hash_map<const Process::ProcessModel*, int> proc_ids;
  int cur_itv_id{};
  int cur_node_id{};
  int cur_proc_id{};
  int cur_cond_id{};
  int cur_trig_id{};
  void finishList()
  {
    if (membersText.endsWith("; "))
      membersText.resize(membersText.size() - 2);
  }

  void addProcess()
  {
    cur_proc_id++;
  }

  template<typename... Args>
  void addMember(fmt::format_string<Args...> str, Args&&... args) {
    membersText += QString(2 * indent, ' ') +
                   QString::fromStdString(
                       fmt::format(str, std::forward<Args>(args)...)
                       ) + "\n";
  }
  template<typename... Args>
  void addCtor(fmt::format_string<Args...> str, Args&&... args) {
    ctorText += QString(4 * indent, ' ') +
                QString::fromStdString(
                    fmt::format(str, std::forward<Args>(args)...)
                    ) + "\n";
  }


  // void addLine(const char* str) {
  //   membersText += QString(2 * indent, ' ') + str + "\n";
  // }

public:
  int indent{};
  QString membersText;
  QString ctorText;
  CPPVisitorStructHeader() = default;

  void operator()(const Scenario::IntervalModel& itv)
  {
    for(auto& proc : itv.processes) {
      if (auto scenar = qobject_cast<Scenario::ProcessModel*>(&proc))
      {
        (*this)(*scenar);
      }
      else
      {
        addMember("std::shared_ptr<ossia::node_process_raw> {} = std::make_shared<ossia::node_process_raw>(process_exec_table[{}]);", Pico::id(proc), cur_proc_id);
      }

      addCtor("{}->add_time_process({});", Pico::id(itv), Pico::id(proc));
      addProcess();
    }
  }

  void operator()(const Scenario::ProcessModel& proc)
  {
    // int id = ++cur_proc_id;
    // proc_ids.insert({&proc, id});
    addMember("std::shared_ptr<ossia::scenario> {} = std::make_shared<ossia::scenario>();", Pico::id(proc));

    for (auto& ts : proc.timeSyncs)
    {
      if(&ts == &proc.startTimeSync())
      {
        addMember("const std::shared_ptr<ossia::time_sync>& {} = {}->get_start_time_sync();", Pico::id(ts), Pico::id(proc));
      }
      else
      {
        addMember("std::shared_ptr<ossia::time_sync> {} = std::make_shared<ossia::time_sync>();", Pico::id(ts));
        addCtor("{}->set_expression(ossia::expressions::make_expression_true());", Pico::id(ts));
        addCtor("{}->add_time_sync({});", Pico::id(proc), Pico::id(ts));
      }
    }

    for (auto& ev : proc.events)
    {
      // if(&ev == &proc.startEvent())
      // {
      //   // Does not seem to be the case anymore:
      //   // addCtor("const std::shared_ptr<ossia::time_event>& {} = *{}->get_start_time_sync()->get_time_events().begin();", Pico::id(ev), Pico::id(proc));
      // }
      // else
      {
        const auto& parent_ts_id = Pico::id(Scenario::parentTimeSync(ev, proc));
        addMember("std::shared_ptr<ossia::time_event> {} = std::make_shared<ossia::time_event>(ossia::time_event::exec_callback{{}}, "
            "*{}, "
            "ossia::expressions::make_expression_true());", Pico::id(ev), parent_ts_id);
        addCtor("{}->insert({}->get_time_events().end(), {});", parent_ts_id, parent_ts_id, Pico::id(ev));
      }
    }

    for (auto& itv : proc.intervals)
    {
      const auto& id = Pico::id(itv);
      addMember("std::shared_ptr<ossia::time_interval> {} = ossia::time_interval::create({{}}, *{}, *{}, {}_tv, {}_tv, {}_tv);",
                id,
                Pico::id(Scenario::startEvent(itv, proc)),
                Pico::id(Scenario::endEvent(itv, proc)),
                itv.duration.defaultDuration().impl,
                itv.duration.minDuration().impl,
                itv.duration.maxDuration().impl
                );
      if(itv.graphal()) {
        addCtor("{}->graphal = true;", id);
      }
      addCtor("{}->add_time_interval({});", Pico::id(proc), id);

      (*this)(itv);
    }
  }
};

// clang-format on

template <typename Container>
int index_in_container_if(Container& vec, auto func) noexcept
{
  auto it = std::find_if(vec.begin(), vec.end(), std::move(func));
  if (it != vec.end())
    return std::distance(vec.begin(), it);
  else
    return -1;
}

class GraphVisitorStructHeader
{
public:
  int cur_proc_id{};
  void finishList()
  {
    if (membersText.endsWith("; "))
      membersText.resize(membersText.size() - 2);
  }

  void addProcess() { cur_proc_id++; }

  template <typename... Args>
  void addMember(fmt::format_string<Args...> str, Args&&... args)
  {
    membersText += QString(2 * indent, ' ')
                   + QString::fromStdString(
                       fmt::format(str, std::forward<Args>(args)...))
                   + "\n";
  }

  template <typename... Args>
  void addCtor(fmt::format_string<Args...> str, Args&&... args)
  {
    ctorText += QString(4 * indent, ' ')
                + QString::fromStdString(
                    fmt::format(str, std::forward<Args>(args)...))
                + "\n";
  }

  template <typename... Args>
  void addExec(fmt::format_string<Args...> str, Args&&... args)
  {
    execText += QString(4 * indent, ' ')
                + QString::fromStdString(
                    fmt::format(str, std::forward<Args>(args)...))
                + "\n";
  }

public:
  int indent{};
  QString membersText;
  QString ctorText;
  QString execText;
  GraphVisitorStructHeader() = default;
  struct W
  {
    std::unique_ptr<Process::CodeWriter> code;
    int index;
  };
  std::vector<W> writers;

  void operator()(const Scenario::IntervalModel& itv)
  {
    for (auto& proc : itv.processes)
    {
      if (auto scenar = qobject_cast<Scenario::ProcessModel*>(&proc))
      {
        (*this)(*scenar);
      }

      if (auto writer = proc.codeWriter(Process::CodeFormat::Cpp))
      {
        writers.push_back(W{std::move(writer), cur_proc_id});
        writers.back().code->variable = Pico::id(proc);
      }

      addProcess();
    }
  }

  void operator()(const Scenario::ProcessModel& proc)
  {
    for (auto& itv : proc.intervals)
    {
      (*this)(itv);
    }
  }

  void postprocess(const score::DocumentContext& context)
  {
    for (auto& w : writers)
    {
      auto& wr = w.code;
      auto& proc = wr->self;

      const auto& pid = wr->variable;

      addMember(
          "{} {} = {{\n    {}\n  }};", wr->typeName(), pid, wr->initializer());

      // addCtor("{}.prepare(...)";

      // Execution semantics:
      {
        // 0. If the process needs to run...
        addExec("{{");
        indent++;
        addExec("// Process: {} ({})", pid, typeid(proc).name());
        addExec(
            "for(const auto& g_tick : process_exec_table[{}]) {{", w.index);
        indent++;
        addExec("bool cable_read = false;");

        // 1. Read inputs
        for (auto inl : proc.inlets())
        {
          // 1.1 Read the cables
          for (auto cable : inl->cables())
          {
            Process::Outlet* out = &cable.find(context).source().find(context);
            auto* parent = safe_cast<Process::ProcessModel*>(out->parent());
            if (int index_of_parent = index_in_container_if(
                    writers, [&](auto& w) { return &w.code->self == parent; });
                index_of_parent != -1)
            {
              addExec(
                  "{{ if (!process_exec_table[{}].empty()) {{ value_adapt({}, "
                  "{}); "
                  "cable_read = true; }} }}",
                  writers[index_of_parent].index,
                  wr->accessInlet(inl->id()),
                  writers[index_of_parent].code->accessOutlet(out->id()));
            }
            else
            {
              // FIXME.. wtf?
              SCORE_TODO;
            }
          }

          // 2. No cable read: read from device
          if (!inl->address().address.device.isEmpty())
          {
            auto addr = inl->address().address.toString().toStdString();
            auto var = wr->accessInlet(inl->id());
            addExec(
                "{{ if(!cable_read) do_device_read<\"{}\">({}); }}",
                addr,
                var);
          }
        }

        // 2. Process

        addExec("{}", wr->execute());
        // 3. Write outputs ?

        {
          for (auto inl : proc.outlets())
          {
            if (!inl->address().address.device.isEmpty())
            {
              auto addr = inl->address().address.toString().toStdString();
              auto var = wr->accessOutlet(inl->id());
              addExec("{{ do_device_write<\"{}\">({}); }}", addr, var);
            }
          }
        }

        indent--;
        addExec("}}");
        addExec("process_exec_table[{}].clear();", w.index);
        indent--;
        addExec("}}");
      }
    }
  }
};

std::string scenarioToCPPStruct(const Scenario::ProcessModel& s)
{
  QString final;
  // 1. The scenario
  {
    CPPVisitorStructHeader m;
    m.indent = 1;
    m(s);

    final += QStringLiteral(
                 "std::vector<ossia::token_request> process_exec_table[%1] = "
                 "{ };\n")
                 .arg(m.cur_proc_id);

    final += "struct scenario_t {\n";
    final += m.membersText;
    final += "\n";
    final += "  scenario_t() {\n";
    final += m.ctorText;
    final += "\n";
    final += "  }\n";
    final += "} scenario;\n";
  }
  // 2. The process graph

  {
    GraphVisitorStructHeader m;
    m.indent = 1;
    m(s);
    m.postprocess(score::IDocument::documentContext(s));

    final += "struct graph_t {\n";
    final += m.membersText;
    final += "\n";
    final += "  graph_t() {\n";
    final += m.ctorText;
    final += "\n";
    final += "  }\n";
    final += "  void execute() {\n";
    final += m.execText;
    final += "\n";
    final += "  }\n";
    final += "} graph;\n";
  }
  return final.toStdString();
}
}
