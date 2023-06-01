#pragma once

#include <Pico/DeviceGraph.hpp>
#include <Pico/DeviceIO.hpp>

#include <vector>
namespace Process
{
class ProcessModel;
}
namespace Pico
{
struct Device;
class SourcePrinter
{
public:
  QString print(
      const Device& device,
      const score::DocumentContext& context,
      const GraphTasks& components);
  QString printDeviceInitialization(const Device& device);
  QString printTask(
      const Device& device,
      const score::DocumentContext& context,
      const std::vector<Process::ProcessModel*>& procs);
};
}
