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
class BasicSourcePrinter
{
public:
  QString print(
      const Device& device, const score::DocumentContext& context,
      const Graph& components);
  QString printDeviceInitialization(const Device& device);
  QString printDataModel(const Device& device, const Graph& components);
  QString printDeviceCommunication(
      const Device& device, const score::DocumentContext& context,
      const Graph& components);
  QString printTask(
      const Device& device, const score::DocumentContext& context,
      const std::vector<Process::ProcessModel*>& procs);
};
}
