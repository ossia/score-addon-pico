#include "ApplicationPlugin.hpp"

#include <Engine/ApplicationPlugin.hpp>
#include <Execution/BaseScenarioComponent.hpp>

#include <score/actions/Menu.hpp>
#include <score/actions/MenuManager.hpp>
#include <score/plugins/documentdelegate/plugin/DocumentPluginCreator.hpp>

#include <QApplication>
#include <QMenu>

#include <Pico/Pruner.hpp>
#include <Pico/SourcePrinter.hpp>
#include <Scenario/Application/Menus/TextDialog.hpp>
#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>

namespace Pico
{

score::GUIElements AppPlug::makeGUIElements()
{
  auto& m = context.menus.get().at(score::Menus::Export());
  QMenu* menu = m.menu();
  auto pouco = new QAction{QObject::tr("Scenario to Arduino"), menu};

  QObject::connect(
      pouco,
      &QAction::triggered,
      [&]()
      {
        auto doc = currentDocument();
        if (!doc)
          return;
        Scenario::ScenarioDocumentModel& base
            = score::IDocument::get<Scenario::ScenarioDocumentModel>(*doc);

        const auto& baseInterval = base.baseScenario().interval();
        if (baseInterval.processes.size() == 0)
          return;

        Pruner p{doc->context()};
        QString text = p.process(baseInterval);
        // QString text
        //     = stal::generateReactiveIS(base.baseScenario(), baseInterval);
        Scenario::TextDialog dial(text, qApp->activeWindow());
        dial.exec();
      });
  menu->addAction(pouco);
  qDebug() << "ayy wtf" << menu << pouco;
  return {};
}
/*
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
*/

void AppPlug::on_createdDocument(score::Document& doc)
{
  //score::addDocumentPlugin<DocPlug>(doc);
}

}
