#include "Pruner.hpp"

#include <Scenario/Document/Interval/IntervalModel.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <QFile>

#include <Pico/BasicSourcePrinter.hpp>
#include <Pico/ESPSourcePrinter.hpp>
#include <Pico/SourcePrinter.hpp>
#include <fmt/format.h>

namespace Pico
{

ProcessScenario::ProcessScenario(const score::DocumentContext& doc)
    : context{doc}
{
}

QString ProcessScenario::process(const Scenario::IntervalModel& root)
{
  QString text;
  scenario.clear();

  // 0. Generate the code for the scenario
  for (auto& proc : root.processes)
  {
    if (auto scenar = qobject_cast<Scenario::ProcessModel*>(&proc))
    {
      scenario += Pico::scenarioToCPPStruct(*scenar);
      break;
    }
  }

  text = QString::fromStdString(scenario);
  return text;
}

////////////////////////////////////////////////////////////////////////////

ComponentBasedSplit::ComponentBasedSplit(const score::DocumentContext& doc)
    : context{doc}
{
}
std::vector<std::pair<QString, QString>>
ComponentBasedSplit::process(const Scenario::IntervalModel& root)
{
  std::vector<std::pair<QString, QString>> res;
  devices.clear();
  scenario.clear();

  // 1. Scan all the devices and scenarios
  (*this)(root, QString{});

  // 2. For each device, generate a graph
  for (std::pair<const QString, Device>& device : devices)
  {
    device.second.name = device.first;
    device.second.ios.push_back(DeviceIO{
        .type = DeviceIO::ADC, .direction = DeviceIO::Input, .pin = 0});
    device.second.ios.push_back(DeviceIO{
        .type = DeviceIO::PWM, .direction = DeviceIO::Output, .pin = 21});

    auto [components, order] = processGraph(context, device.second.processes);
    BasicSourcePrinter p;
    QString src = p.print(device.second, context, components);
    QString filename = device.first;
    res.emplace_back(filename, src);
  }
  return res;
}

void ComponentBasedSplit::operator()(
    const Scenario::IntervalModel& cst,
    QString group)
{
  // FIXME use Interval::networkGroup() when it's merged in master
  if (const auto& label = cst.metadata().getLabel(); label.contains("pico"))
    group = label;

  for (auto& proc : cst.processes)
  {
    if (auto scenar = qobject_cast<Scenario::ProcessModel*>(&proc))
    {
      // Scan for the devices
      for (auto& itv : scenar->intervals)
      {
        (*this)(itv, group);
      }
    }
    else
    {
      (*this)(proc, group);
    }
  }
}

void ComponentBasedSplit::operator()(
    Process::ProcessModel& comp,
    QString group)
{
  // Check where this process executes - for now we just use the label of the parent interval
  devices[group].processes.push_back(&comp);
}

////////////////////////////////////////////////////////////////////////////

IntervalSplit::IntervalSplit(const score::DocumentContext& doc)
    : context{doc}
{
}

QString IntervalSplit::process(const Scenario::IntervalModel& root)
{
  devices.clear();
  scenario.clear();

  // 1. Scan all the intervals with processes and create the tasks
  GraphTasks to_process;
  auto itvs = root.findChildren<Scenario::IntervalModel*>();
  for (auto& itv : itvs)
  {
    if (!itv->processes.empty() && !itv->metadata().getLabel().isEmpty())
    {
      // Create a new task
      GraphTasks::value_type v;

      v.taskName = itv->metadata().getLabel();

      // Add the processes
      for (auto& p : itv->processes)
        v.processes.push_back(&p);

      // Topo sort them
      auto [components, order] = processGraph(context, v.processes);
      v.processes = std::move(order);

      // Save the task
      to_process.push_back(std::move(v));
    }
  }

  // 2. Print the tasks
  std::string task_text;
  Device c;
  c.name = "esp8266";
  for (auto& task : to_process)
  {
    BasicSourcePrinter p;
    QString taskCode = p.printTask(c, context, task.processes);

    {
      task_text += "\n";
      task_text += fmt::format(
          "static void {}()\n{{\n{}\n}}\n",
          task.taskName.toStdString(),
          taskCode.toStdString());
    }
  }

  return QString::fromStdString(task_text);
}
}
