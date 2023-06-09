#pragma once

#include <score/plugins/application/GUIApplicationPlugin.hpp>

namespace Pico
{

using Group = QString;
class AppPlug : public score::GUIApplicationPlugin
{
public:
  using GUIApplicationPlugin::GUIApplicationPlugin;

  void on_createdDocument(score::Document& doc) override;

  score::GUIElements makeGUIElements() override;
};

}
