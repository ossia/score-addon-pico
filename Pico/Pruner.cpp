#include "Pruner.hpp"

#include <QFile>

#include <Pico/BasicSourcePrinter.hpp>
#include <Pico/ESPSourcePrinter.hpp>
#include <Pico/SourcePrinter.hpp>
#include <Scenario/Document/Interval/IntervalModel.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

namespace Pico
{

Pruner::Pruner(const score::DocumentContext& doc)
    : context{doc}
{
}

QString Pruner::process(const Scenario::IntervalModel& root)
{
  QString text;
  devices.clear();
  scenario.clear();

  // 1. Scan all the devices and scenarios
  (*this)(root, QString{});

  // 2. For each device, generate a graph
  for (std::pair<const QString, Device>& device : devices)
  {
    qDebug() << "Device: " << device.first << ": "
             << device.second.processes.size();
    device.second.name = device.first;
    device.second.ios.push_back(DeviceIO{
        .type = DeviceIO::ADC, .direction = DeviceIO::Input, .pin = 0});
    device.second.ios.push_back(DeviceIO{
        .type = DeviceIO::PWM, .direction = DeviceIO::Output, .pin = 21});

    auto components = device.second.processGraph(context);
    BasicSourcePrinter p;
    QString src = p.print(device.second, context, components);
    QString filename = QString("/tmp/picodevice.%1.cpp").arg(device.first);
    {
      QFile f(filename);
      f.open(QIODevice::WriteOnly);
      f.write(src.toUtf8());
    }
    system(("clang-format -i " + filename.toStdString()).c_str());
    {
      QFile f(filename);
      f.open(QIODevice::ReadOnly);
      text = f.readAll();
      qDebug().noquote().nospace() << text;
    }

    {
      QFile f("/tmp/scenario.cpp");
      f.open(QIODevice::WriteOnly);
      f.write(scenario.c_str(), scenario.length());
    }
  }
  system("code /tmp/scenario.cpp");
  return text;
}

void Pruner::operator()(const Scenario::IntervalModel& cst, QString group)
{
  // FIXME use Interval::networkGroup() when it's merged in master
  if (const auto& label = cst.metadata().getLabel(); label.contains("pico"))
    group = label;

  for (auto& proc : cst.processes)
  {
    if (auto scenar = qobject_cast<Scenario::ProcessModel*>(&proc))
    {
      // Generate the code for the scenario
      scenario += Pico::scenarioToCPP(*scenar);

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

void Pruner::operator()(Process::ProcessModel& comp, QString group)
{
  // Check where this process executes - for now we just use the label of the parent interval
  devices[group].processes.push_back(&comp);
}

}
