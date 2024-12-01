#pragma once
#include <ossia/detail/flat_map.hpp>

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
class ProcessScenario
{
public:
  const score::DocumentContext& context;
  std::string scenario;
  explicit ProcessScenario(const score::DocumentContext& doc);

  QString process(const Scenario::IntervalModel& root);
  void operator()(const Scenario::IntervalModel& cst);
};

class ComponentBasedSplit
{
public:
  const score::DocumentContext& context;
  std::string scenario;
  Devices devices;
  explicit ComponentBasedSplit(const score::DocumentContext& doc);

  std::vector<std::pair<QString, QString>> process(const Scenario::IntervalModel& root);
  void operator()(const Scenario::IntervalModel& cst, QString group);
  void operator()(Process::ProcessModel& comp, QString group);
};

class IntervalSplit
{
public:
  const score::DocumentContext& context;
  std::string scenario;
  Devices devices;
  explicit IntervalSplit(const score::DocumentContext& doc);

  QString process(const Scenario::IntervalModel& root);
  void operator()(const Scenario::IntervalModel& cst, QString group);
  void operator()(Process::ProcessModel& comp, QString group);
};
}
