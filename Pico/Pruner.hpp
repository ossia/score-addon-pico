#pragma once
#include <Pico/DeviceGraph.hpp>
namespace Process
{
class ProcessModel;
}
namespace Scenario
{
class IntervalModel;
class ProcessModel;
}
namespace Pico
{
class Pruner
{
public:
  const score::DocumentContext& context;
  std::string scenario;
  Devices devices;
  explicit Pruner(const score::DocumentContext& doc);

  QString process(const Scenario::IntervalModel& root);
  void operator()(const Scenario::IntervalModel& cst, QString group);
  void operator()(Process::ProcessModel& comp, QString group);
};
}
