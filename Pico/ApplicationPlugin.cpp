#include "ApplicationPlugin.hpp"

#include <Engine/ApplicationPlugin.hpp>
#include <Execution/BaseScenarioComponent.hpp>

#include <score/actions/Menu.hpp>
#include <score/actions/MenuManager.hpp>
#include <score/plugins/documentdelegate/plugin/DocumentPluginCreator.hpp>

#include <QAction>
#include <QDialog>
#include <QMainWindow>
#include <QMenu>

#include <Pico/ExportDialog.hpp>

namespace Pico
{

score::GUIElements AppPlug::makeGUIElements()
{
  auto& m = context.menus.get().at(score::Menus::Export());
  QMenu* menu = m.menu();
  auto act = new QAction{QObject::tr("Export to code"), menu};
  act->setShortcutContext(Qt::ShortcutContext::ApplicationShortcut);
  act->setShortcut(QKeySequence("Alt+Shift+E"));
  QObject::connect(act, &QAction::triggered, this, &AppPlug::requestExport);
  menu->addAction(act);

  return {};
}

void AppPlug::requestExport()
{
  if(auto win = score::GUIAppContext().mainWindow)
  {
    if(!m_dialog)
      m_dialog = new ExportDialog{*this, win};
    m_dialog->show();
  }
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

} // namespace Pico
