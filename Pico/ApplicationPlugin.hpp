#pragma once

#include <score/plugins/application/GUIApplicationPlugin.hpp>

namespace Pico {
class ExportDialog;
class AppPlug : public QObject, public score::GUIApplicationPlugin
{
public:
  using GUIApplicationPlugin::GUIApplicationPlugin;

  void on_createdDocument(score::Document& doc) override;
  void on_loadedDocument(score::Document& doc) override;

  score::GUIElements makeGUIElements() override;

  void requestExport();

private:
  ExportDialog* m_dialog{};
};

} // namespace Pico
