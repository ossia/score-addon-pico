#pragma once

#include <Process/Process.hpp>

#include <map>
#include <vector>

namespace Pico
{
struct DeviceIO
{
  enum Type
  {
    GPIO,
    PWM
  } type{};
  enum Direction
  {
    Input,
    Output
  } direction{}; // relative to the board: a LED is likely output

  int pin{};

  std::map<QString, QString> properties;

  QString printInitialization();
};

struct Device
{
  QString name;
  std::vector<Process::ProcessModel*> processes;
  std::vector<DeviceIO> ios;

  void processGraph(const score::DocumentContext& context);
  QString printDeviceInitialization();
  QString printTask(
      const score::DocumentContext& context,
      const std::vector<Process::ProcessModel*>& procs);
};
using Devices = std::map<QString, Device>;

}
