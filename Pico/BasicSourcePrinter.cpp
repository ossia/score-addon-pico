#include "BasicSourcePrinter.hpp"

#include <Process/CodeWriter.hpp>
#include <Process/Dataflow/Cable.hpp>
#include <Process/Dataflow/Port.hpp>
#include <Process/Process.hpp>

#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>
#define OSSIA_PROTOCOL_SIMPLEIO 1
#include <Protocols/OSC/OSCDevice.hpp>
#include <Protocols/OSCQuery/OSCQueryDevice.hpp>
#include <Protocols/SimpleIO/SimpleIODevice.hpp>

#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/fmt.hpp>

#include <QTextStream>

#include <Pico/LibossiaGenerator.hpp>
#include <Pico/SourcePrinter.hpp>

namespace Pico
{
static const std::string error_string = ":*:ERROR:*:";
static const std::string todo_string = ":*:TODO:*:";
static std::string address_to_model_accessor(const State::Address& addr)
{
  QString device = addr.device;
  ossia::net::sanitize_name(device);

  QString variable_name = addr.path.join("_");
  ossia::net::sanitize_name(variable_name);

  return fmt::format(
      "ossia_model.{}.{}", device.toStdString(), variable_name.toStdString());
}
static std::optional<std::string>
address_to_device_read_index(const State::Address& addr, std::string var)
{
  if(addr.isSet())
    return fmt::format(
        "value_adapt({}.value, {})", address_to_model_accessor(addr), var);
  return std::nullopt;
}

static std::optional<std::string>
address_to_network_read_index(const State::Address& addr, std::string var)
{
  if(addr.isSet())
    return fmt::format(
        "value_adapt({}.value, {})", address_to_model_accessor(addr), var);
  return std::nullopt;
}

static std::optional<std::string>
address_to_device_write_index(const State::Address& addr, std::string var)
{
  if(addr.isSet())
    return fmt::format(
        "value_adapt({0}, {1}.value); {1}.changed = true;", var,
        address_to_model_accessor(addr));
  return std::nullopt;
}

static std::optional<std::string>
address_to_network_write_index(const State::Address& addr, std::string var)
{
  if(addr.isSet())
    return fmt::format(
        "value_adapt({0}, {1}.value); {1}.changed = true;", var,
        address_to_model_accessor(addr));
  return std::nullopt;
}

QString BasicSourcePrinter::printTask(
    const Device& device,
    const score::DocumentContext& context,
    const std::vector<Process::ProcessModel*>& procs)
{
  std::string c;
  c.reserve(2000);
  c += "{\n";

  std::vector<std::unique_ptr<Process::CodeWriter>> codes;

  for (auto* p : procs)
  {
    qDebug() << p->metadata().getName() << p->prettyName();
    auto writer = p->codeWriter(Process::CodeFormat::Cpp);

    if (writer)
    {
      codes.push_back(std::move(writer));
    }
    else
    {
      QString fatalMessage;
      QTextStream outputStream(&fatalMessage);
      outputStream << "Process:" << p->metadata().getName() << typeid(*p).name()
                   << " does not have a code writer";
      outputStream.flush();
      qFatal(qPrintable(fatalMessage));
    }
  }

  // 1. Instantiate the processes
  {
    int index = 0;
    for (auto& wr : codes)
    {
      std::string name = procs[index]->metadata().getName().toStdString();
      std::string init = wr->initializer();
      wr->variable = fmt::format("p{}", index);
      c += fmt::format(
          "static {} {} {{\n{}\n}}; // {}\n",
          wr->typeName(),
          wr->variable,
          init,
          name);
      index++;
    }
  }

  // Start the loop
  // c += "for(;;) { \n";

  // 2. Write the inputs addresses

  c += fmt::format("/// INPUT READ\n\n");
  {
    int process_index = 0;
    for (auto* p : procs)
    {
      auto& wr = codes[process_index];

      c += fmt::format("/// p: {}\n", p->metadata().getName().toStdString());
      for (auto inl : p->inlets())
      {
        if (inl->address().address.device == device.name)
        {
          if(auto res = address_to_device_read_index(
                 inl->address().address, wr->accessInlet(inl->id())))
            c += fmt::format("{{ {}; }}\n", *res);
        }
        else if (!inl->address().address.device.isEmpty())
        {
          if(auto res = address_to_network_read_index(
                 inl->address().address, wr->accessInlet(inl->id())))
            c += fmt::format("{{ {}; }}\n", *res);
        }
      }

      process_index++;
    }
  }

  c += fmt::format("/// GRAPH PROCESSING\n\n");
  // 3. Write down the graph
  {
    int process_index = 0;
    for (auto& wr : codes)
    {
      auto& p = *procs[process_index];
      c += fmt::format("/// p: {}\n", p.metadata().getName().toStdString());
      // Set all the cable inlets
      for (auto inl : p.inlets())
      {
        for (auto cable : inl->cables())
        {
          Process::Outlet* out = &cable.find(context).source().find(context);
          auto* parent = safe_cast<Process::ProcessModel*>(out->parent());
          if (int index_of_parent = ossia::index_in_container(procs, parent);
              index_of_parent != -1)
          {
            c += fmt::format(
                "{{ value_adapt({}, {}); }}\n",
                wr->accessInlet(inl->id()),
                codes[index_of_parent]->accessOutlet(out->id()));
          }
          else
          {
            // FIXME
            SCORE_TODO;
          }
        }
      }

      // run
      c += wr->execute() + "\n";

      process_index++;
    }
  }

  // 4. Write the outputs addresses
  c += fmt::format("/// OUTPUT WRITE\n\n");
  {
    int process_index = 0;
    for (auto* p : procs)
    {
      auto& wr = codes[process_index];

      c += fmt::format("/// p: {}\n", p->metadata().getName().toStdString());
      for (auto inl : p->outlets())
      {
        if (inl->address().address.device == device.name)
        {
          if(auto res = address_to_device_write_index(
                 inl->address().address, wr->accessOutlet(inl->id())))
            c += fmt::format("{{ {}; }}\n", *res);
        }
        else if (!inl->address().address.device.isEmpty())
        {
          if(auto res = address_to_network_write_index(
                 inl->address().address, wr->accessOutlet(inl->id())))
            c += fmt::format("{{ {}; }}\n", *res);
        }
      }

      process_index++;
    }
  }

  // End the loop
  // c += "}\n";

  c += "}\n";

  return QString::fromStdString(c);
}

QString BasicSourcePrinter::print(
    const Device& device, const score::DocumentContext& context, const Graph& g)
{
  // 4. Print source code
  std::string source;
  source += Pico::defaultIncludesGraph();
  source += fmt::format(
      R"(#include "{}.ossia-model.generated.hpp")", device.name.toStdString());

  source += printDeviceInitialization(device).toStdString();
  std::string task_creation;
  int task_n = 0;

  qDebug() << " <<<<>>>> there are " << g.tasks.size() << " components ";
  ;
  for(auto& comp : g.tasks)
  {
    source += "\n";
    source += fmt::format("static void ossia_task_{}() {{\n", task_n);
    source += printTask(device, context, comp.processes).toStdString();
    source += "}\n";

    task_creation += fmt::format(R"_(ossia_task_{0}();)_", task_n);
    task_creation += "\n";
    task_n++;
  }

  // Write the main:

  source += fmt::format(
      R"_(
extern "C" void ossia_run_graph(void)
{{
  {}
}}
)_",
      task_creation);

  return QString::fromStdString(source);
}

static constexpr auto data_model_template = R"_(
struct ossia_model_t
{{
    {}
}} ossia_model;
//  struct
//  {
//    int foo;
//    float bar;
//  } net;
//  struct
//  {
//    int foo;
//    float bar;
//  } dev;
//  struct
//  {
//    uint32_t bar : 1;
//  } net_bitset;
//  struct
//  {
//    uint32_t bar : 1;
//  } dev_bitset;


void do_read(auto& dest, auto& value)
{{
  value_adapt(value, ossia_model.net.foo);
  if constexpr(0) {{ }}
  {}
  
  // else if constexpr(std::string_view{str} == "osc:/foo")
  //   value_adapt(value, ossia_model.net.foo);
}

template <auto str>
void do_device_read(auto& value)
{
  if constexpr(0) {{ }}
  {}
  
}
template <auto str>
void do_network_write(const auto& value)
{
  if constexpr(0) {{ }}
  {}
  
}
template <auto str>
void do_device_write(auto& value)
{
  if constexpr(0) {{ }}
  {}
  
  // else if constexpr(std::string_view{str} == "gpio:/bar")
  // {
  //   value_adapt(ossia_model.dev.bar, value);
  //   ossia_model.dev_bitset.bar = 1;
  // }
}
)_";

QString BasicSourcePrinter::printDeviceInitialization(const Device& device)
{
  QString str;
  return str;
}

QString BasicSourcePrinter::printDeviceCommunication(
    const Device& device, const score::DocumentContext& context, const Graph& components)
{
  auto& device_list = context.plugin<Explorer::DeviceDocumentPlugin>().list();
  QString file;
  // ossia_read_pins
  {
    std::string f;

    f += "void ossia_read_pins() {\n";
    // FIXME codewriter interface on devices
    for(auto& [devname, paths] : components.in_addresses)
    {
      if(auto* d = device_list.findDevice(devname))
      {
        if(auto proto = qobject_cast<Protocols::SimpleIODevice*>(d))
        {
        }
      }
    }
    for(auto& e : device.ios)
    {
      // If device
    }

    f += "}\n";
  }
  // ossia_read_net
  // ossia_run_graph

  // ossia_write_pins
  // ossia_write_net

  return file;
}

QString BasicSourcePrinter::printDataModel(const Device& device, const Graph& g)
{
  QString str;

  // 2. Generate a model struct
  QString model_struct;
  for(const auto& [k, v] : g.merged_addresses)
  {
    QString structdef = "struct {\n";

    for(const auto& vv : v)
    {
      std::string variable_name = vv.join("_").toStdString();
      ossia::net::sanitize_name(variable_name);
      structdef += fmt::format(
          "struct {{ {0} value = {{}}; bool changed = false; }} {1};\n", "float",
          variable_name);
    }
    structdef += "} ";

    QString device_name = k;
    ossia::net::sanitize_name(device_name);
    structdef += k;
    structdef += ";\n";

    model_struct += structdef;
  }

  // 3. Generate the accessor functions

  return QString::fromStdString(fmt::format(
      R"_(#pragma once
struct ossia_model_t
{{
    {}
}} ossia_model;
)_",
      model_struct.toStdString()));
}
}
