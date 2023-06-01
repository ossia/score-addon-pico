#pragma once

#include <score/document/DocumentContext.hpp>

#include <Pico/DeviceIO.hpp>
namespace Process
{
class ProcessModel;
}

namespace Pico
{
using GraphTasks = std::vector<std::vector<Process::ProcessModel*>>;
struct Device
{
  QString name;
  std::vector<Process::ProcessModel*> processes;
  std::vector<DeviceIO> ios;

  GraphTasks processGraph(const score::DocumentContext& context);
};
using Devices = std::map<QString, Device>;

}
