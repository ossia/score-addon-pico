#pragma once

#include <score/document/DocumentContext.hpp>

#include <Pico/DeviceIO.hpp>
namespace Process
{
class ProcessModel;
}

namespace Pico
{
struct Task
{
  QString taskName;
  std::vector<Process::ProcessModel*> processes;
};

using GraphTasks = std::vector<Task>;

struct Graph
{
  GraphTasks tasks;
  std::vector<Process::ProcessModel*> topo_order;
};

Graph processGraph(
    const score::DocumentContext& context,
    const std::vector<Process::ProcessModel*>& processes);
struct Device
{
  QString name;
  std::vector<Process::ProcessModel*> processes;
  std::vector<DeviceIO> ios;
};
using Devices = std::map<QString, Device>;

}
