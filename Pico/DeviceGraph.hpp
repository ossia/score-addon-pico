#pragma once

#include <score/document/DocumentContext.hpp>

#include <ossia/detail/hash_map.hpp>

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

  ossia::hash_map<QString, ossia::flat_set<QStringList>> in_addresses;
  ossia::hash_map<QString, ossia::flat_set<QStringList>> out_addresses;
  ossia::hash_map<QString, ossia::flat_set<QStringList>> merged_addresses;
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
