#include "ApplicationPlugin.hpp"

#include <Engine/ApplicationPlugin.hpp>
#include <Execution/BaseScenarioComponent.hpp>

#include <score/plugins/documentdelegate/plugin/DocumentPluginCreator.hpp>

#include <QFile>

#include <Pico/DeviceGraph.hpp>
#include <Pico/SourcePrinter.hpp>
#include <Scenario/Document/Interval/IntervalExecution.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>
#include <Scenario/Execution/ScenarioExecution.hpp>
#include <Scenario/Process/ScenarioExecution.hpp>

namespace Pico
{

class Pruner
{
public:
  const score::DocumentContext& context;
  Devices devices;
  explicit Pruner(const score::DocumentContext& doc)
      : context{doc}
  {
  }

  void operator()(
      const Execution::Context& ctx,
      Execution::BaseScenarioElement& scenar) noexcept
  {
    devices.clear();
    (*this)(scenar.baseInterval(), {});
    for (auto& device : devices)
    {
      qDebug() << "Device: " << device.first << ": "
               << device.second.processes.size();
      device.second.name = device.first;
      device.second.ios.push_back(DeviceIO{
          .type = DeviceIO::ADC, .direction = DeviceIO::Input, .pin = 0});
      device.second.ios.push_back(DeviceIO{
          .type = DeviceIO::PWM, .direction = DeviceIO::Output, .pin = 21});

      auto components = device.second.processGraph(ctx.doc);
      SourcePrinter p;
      QString src = p.print(device.second, ctx.doc, components);
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
        QString src = f.readAll();
        qDebug().noquote().nospace() << src;
      }
    }
  }

  void operator()(Execution::IntervalComponent& cst, QString group)
  {
    // FIXME use Interval::networkGroup() when it's merged in master
    if (const auto& label = cst.interval().metadata().getLabel();
        label.contains("pico"))
      group = label;

    for (auto& proc : cst.processes())
    {
      if (auto scenar_scan
          = qobject_cast<Execution::ScenarioComponentBase*>(proc.second.get()))
      {
        for (auto& itv : scenar_scan->intervals())
        {
          (*this)(*itv.second, group);
        }
      }
      else
      {
        (*this)(*proc.second, group);
      }
    }
  }

  void operator()(Execution::ProcessComponent& comp, QString group)
  {
    // Check where this process executes - for now we just use the label of the parent interval
    devices[group].processes.push_back(&comp.process());
  }
};

class DocPlug : public score::DocumentPlugin
{
public:
  DocPlug(const score::DocumentContext& doc, QObject* parent)
      : score::DocumentPlugin{doc, "RemoteControl::DocumentPlugin", parent}
  {
    auto& plug
        = m_context.app.guiApplicationPlugin<Engine::ApplicationPlugin>();
    plug.execution().register_execution_filter(Pruner{doc});
  }

  ~DocPlug() { }

  void play() { }
};

void AppPlug::on_createdDocument(score::Document& doc)
{
  score::addDocumentPlugin<DocPlug>(doc);
}

}
