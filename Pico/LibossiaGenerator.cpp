#include "LibossiaGenerator.hpp"

#include <score/model/path/Path.hpp>

#include <ossia/detail/algorithms.hpp>
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
  template <typename T>
  std::string id(const T& c)
  {
    QString tmp = Path<T>(c).unsafePath().toString();
    tmp.remove("Scenario::ScenarioDocumentModel.1/Scenario::BaseScenario.0/");
    tmp.remove("Scenario::");
    tmp.replace("IntervalModel", "itv");
    tmp.replace("TimeSyncModel", "ts");
    tmp.replace("EventModel", "ev");
    tmp.replace("StateModel", "st");
    tmp.replace("Scenario", "sc");
    tmp.replace('/', '_');
    tmp.replace('.', '_');
    if(tmp.back() == '_')
      tmp.resize(tmp.size() - 1);
    return tmp.toStdString();
  }

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

  struct with_brace
  {
    CPPVisitor* self;
    with_brace(CPPVisitor* s) : self{s}
    {
      self->addLine("{");
      self->indent++;
    }
    ~with_brace()
    {
      self->addLine("}");
      self->indent--;
    }
  };

  void operator()(const Scenario::ProcessModel& proc)
  {
    int id = ++cur_proc_id;
    proc_ids.insert({&proc, id});
    addLine("auto {} = std::make_shared<ossia::scenario>();", this->id(proc));
    addLine("");

    for (auto& ts : proc.timeSyncs)
    {
      if(&ts == &proc.startTimeSync())
      {
        addLine("const auto& {} = {}->get_start_time_sync();", this->id(ts), this->id(proc));
      }
      else
      {
        addLine("auto {} = std::make_shared<ossia::time_sync>();", this->id(ts));
        addLine("{}->set_expression(ossia::expressions::make_expression_true());", this->id(ts));
        addLine("{}->add_time_sync({});", this->id(proc), this->id(ts));
      }
    }

    addLine("");
    for (auto& ev : proc.events)
    {
      if(&ev == &proc.startEvent())
      {
        addLine("const auto& {} = *{}->get_start_time_sync()->get_time_events().begin();", this->id(ev), this->id(proc));
      }
      else
      {
        const auto& parent_ts_id = this->id(Scenario::parentTimeSync(ev, proc));
        addLine("auto {} = std::make_shared<ossia::time_event>(ossia::time_event::exec_callback{{}}, "
            "*{}, "
            "ossia::expressions::make_expression_true());", this->id(ev), parent_ts_id);
        addLine("{}->insert({}->get_time_events().end(), {});", parent_ts_id, parent_ts_id, this->id(ev));
      }
    }

    addLine("");
    for (auto& itv : proc.intervals)
    {
      const auto& id = this->id(itv);
      addLine("auto {} = ossia::time_interval::create({{}}, *{}, *{}, {}_tv, {}_tv, {}_tv);",
              id,
              this->id(Scenario::startEvent(itv, proc)),
              this->id(Scenario::endEvent(itv, proc)),
              itv.duration.defaultDuration().impl,
              itv.duration.minDuration().impl,
              itv.duration.maxDuration().impl
              );
       if(itv.graphal())
       addLine("{}.graphal = true;", id);
       addLine("{}->add_time_interval({});", this->id(proc), id);
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
