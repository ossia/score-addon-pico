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
#include <Protocols/SimpleIO/SimpleIOSpecificSettings.hpp>

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
  {
    return fmt::format(
        "value_adapt({0}, {1}.value)", var, address_to_model_accessor(addr));
  }
  return std::nullopt;
}

static std::optional<std::string>
address_to_network_read_index(const State::Address& addr, std::string var)
{
  if(addr.isSet())
  {
    return fmt::format(
        "value_adapt({0}, {1}.value)", var, address_to_model_accessor(addr));
  }
  return std::nullopt;
}

static std::optional<std::string>
address_to_device_write_index(const State::Address& addr, std::string var)
{
  if(addr.isSet())
  {
    return fmt::format(
        "value_adapt({0}.value, {1}); {0}.changed = true;",
        address_to_model_accessor(addr), var);
  }
  return std::nullopt;
}

static std::optional<std::string>
address_to_network_write_index(const State::Address& addr, std::string var)
{
  if(addr.isSet())
  {
    return fmt::format(
        "value_adapt({0}.value, {1}); {0}.changed = true;",
        address_to_model_accessor(addr), var);
  }
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
      c += fmt::format("\n");
      wr->variable = fmt::format("p{}", index);
      std::string name = procs[index]->metadata().getName().toStdString();
      std::string type = wr->typeName();
      std::string init = wr->initializer();
      c += fmt::format(
          "static {} {} {{\n{}\n}}; // {}\n",
          type,
          wr->variable,
          init,
          name);

      // Callback output storage (for processes with halp::callback outputs)
      c += fmt::format(
          "static avnd_callback_storage_t<{}> {}_cbs;\n",
          type,
          wr->variable);

      // One-time initialization: prepare(), dynamic port resizing, callback binding
      c += fmt::format(
          "static bool prepared_{0} = false;\n"
          "if(!prepared_{0}) {{\n"
          "  do_prepare({0}, g_setup);\n",
          wr->variable);

      // Initialize dynamic ports (resize port vectors to fixed compile-time size)
      if(auto post = wr->postInitialize(); !post.empty())
        c += "  " + post;

      // Bind callback outputs to storage
      c += fmt::format(
          "  avnd_init_callbacks({0}, {0}_cbs);\n",
          wr->variable);

      c += fmt::format(
          "  prepared_{0} = true;\n"
          "}}\n",
          wr->variable);

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
      c += fmt::format("\n");
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

      // Clean callback storage before execution, then run
      c += fmt::format(
          "avnd_clean_callback_outputs<{}>({}_cbs); ",
          wr->typeName(), wr->variable);
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
  g_tick.update_timings();
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

void BasicSourcePrinter::analyzeDeviceTypes(
    const score::DocumentContext& context, const Graph& g)
{
  namespace sio = Protocols::SimpleIO;
  auto& device_list = context.plugin<Explorer::DeviceDocumentPlugin>().list();

  for(auto& [dev_name, paths] : g.merged_addresses)
  {
    auto* d = device_list.findDevice(dev_name);
    if(!d)
      continue;

    auto proto = qobject_cast<Protocols::SimpleIODevice*>(d);
    if(!proto)
      continue;

    const auto& set
        = proto->settings()
              .deviceSpecificSettings.value<Protocols::SimpleIOSpecificSettings>();

    QString sanitized_dev = dev_name;
    ossia::net::sanitize_name(sanitized_dev);

    for(auto& port : set.ports)
    {
      std::string variable_name = port.path.split("/").join("_").toStdString();
      ossia::net::sanitize_name(variable_name);

      std::string key = fmt::format("{}.{}", sanitized_dev.toStdString(), variable_name);

      if(auto ptr = ossia::get_if<sio::Neopixel>(&port.control))
        model_field_types[key] = "std::vector<float>";
    }
  }
}

// Compute the model accessor for a SimpleIO port, matching the naming in printDataModel
// E.g. for device "MyBoard", port path "pot1/SIG" -> "ossia_model.MyBoard.pot1_SIG"
static std::string model_accessor(const QString& device_name, const Protocols::SimpleIO::Port& port)
{
  QString dev = device_name;
  ossia::net::sanitize_name(dev);

  std::string variable_name = port.path.split("/").join("_").toStdString();
  ossia::net::sanitize_name(variable_name);

  return fmt::format("ossia_model.{}.{}", dev.toStdString(), variable_name);
}

// Generate init/read/write code for a single SimpleIO device
static void generateDeviceIO(
    const QString& dev_name,
    const Protocols::SimpleIOSpecificSettings& set,
    std::string& globals,
    std::string& init,
    std::string& read,
    std::string& write)
{
  namespace sio = Protocols::SimpleIO;

  for(auto& port : set.ports)
  {
    auto acc = model_accessor(dev_name, port);

    if(auto ptr = ossia::get_if<sio::GPIO>(&port.control))
    {
      init += fmt::format(
          "  pinMode({}, {});\n", ptr->line, ptr->direction ? "OUTPUT" : "INPUT");

      if(!ptr->direction) // input
        read += fmt::format("  {0}.value = digitalRead({1});\n", acc, ptr->line);
      else // output
        write += fmt::format(
            "  if({0}.changed) {{ digitalWrite({1}, {0}.value); {0}.changed = false; }}\n",
            acc, ptr->line);
    }
    else if(auto ptr = ossia::get_if<sio::ADC>(&port.control))
    {
      read += fmt::format(
          "  {0}.value = analogRead({1}) / 4095.f;\n", acc, ptr->channel);
    }
    else if(auto ptr = ossia::get_if<sio::PWM>(&port.control))
    {
      write += fmt::format(
          "  if({0}.changed) {{ analogWrite({1}, (int)({0}.value * 255)); {0}.changed = false; }}\n",
          acc, ptr->channel);
    }
    else if(auto ptr = ossia::get_if<sio::DAC>(&port.control))
    {
      write += fmt::format(
          "  if({0}.changed) {{ dacWrite({1}, (int)({0}.value * 255)); {0}.changed = false; }}\n",
          acc, ptr->channel);
    }
    else if(auto ptr = ossia::get_if<sio::Neopixel>(&port.control))
    {
      globals += fmt::format(
          "Adafruit_NeoPixel ossia_neopixel_{0}({1}, {0}, NEO_GRB + NEO_KHZ800);\n",
          ptr->pin, ptr->num_pixels);

      init += fmt::format("  ossia_neopixel_{}.begin();\n", ptr->pin);

      // Write pixel data from the float array
      // The array can be: N floats (grayscale) or 3*N floats (RGB per pixel)
      write += fmt::format(
          "  if({0}.changed) {{\n"
          "    auto& pv = {0}.value;\n"
          "    const int n = {1};\n"
          "    if((int)pv.size() >= n * 3) {{\n"
          "      uint8_t* buf = ossia_neopixel_{2}.getPixels();\n"
          "      for(int i = 0; i < n; i++) {{\n"
          "        buf[i * 3 + 0] = (uint8_t)(pv[i * 3 + 1] * 255);\n"  // NeoPixel GRB order
          "        buf[i * 3 + 1] = (uint8_t)(pv[i * 3 + 0] * 255);\n"
          "        buf[i * 3 + 2] = (uint8_t)(pv[i * 3 + 2] * 255);\n"
          "      }}\n"
          "    }} else {{\n"
          "      int count = ((int)pv.size() < n) ? (int)pv.size() : n;\n"
          "      for(int i = 0; i < count; i++) {{\n"
          "        uint8_t v = (uint8_t)(pv[i] * 255);\n"
          "        ossia_neopixel_{2}.setPixelColor(i, ossia_neopixel_{2}.Color(v, v, v));\n"
          "      }}\n"
          "    }}\n"
          "    ossia_neopixel_{2}.show();\n"
          "    {0}.changed = false;\n"
          "  }}\n",
          acc, ptr->num_pixels, ptr->pin);
    }
  }
}

QString BasicSourcePrinter::printDeviceCommunication(
    const Device& device, const score::DocumentContext& context, const Graph& components)
{
  auto& device_list = context.plugin<Explorer::DeviceDocumentPlugin>().list();

  std::string globals;
  std::string init;
  std::string read;
  std::string write;
  bool has_neopixel = false;

  // Iterate over all device names referenced by the graph's addresses
  for(auto& [dev_name, paths] : components.merged_addresses)
  {
    auto* d = device_list.findDevice(dev_name);
    if(!d)
      continue;

    auto proto = qobject_cast<Protocols::SimpleIODevice*>(d);
    if(!proto)
      continue;

    const auto& set
        = proto->settings().deviceSpecificSettings.value<Protocols::SimpleIOSpecificSettings>();

    // Check for neopixels
    for(auto& port : set.ports)
      if(ossia::get_if<Protocols::SimpleIO::Neopixel>(&port.control))
        has_neopixel = true;

    generateDeviceIO(dev_name, set, globals, init, read, write);
  }

  std::string ret;
  ret += "#pragma once\n";
  ret += "#include <Arduino.h>\n";
  if(has_neopixel)
    ret += "#include <Adafruit_NeoPixel.h>\n";
  ret += fmt::format(
      "#include \"{}.ossia-model.generated.hpp\"\n", device.name.toStdString());
  ret += "\n";
  ret += globals;

  ret += fmt::format(
      "\nvoid ossia_init_board() {{\n{}}}\n", init);
  ret += fmt::format(
      "\nvoid ossia_read_pins() {{\n{}}}\n", read);
  ret += fmt::format(
      "\nvoid ossia_write_pins() {{\n{}}}\n", write);

  return QString::fromStdString(ret);
}

QString BasicSourcePrinter::printDataModel(const Device& device, const Graph& g)
{
  QString str;

  // 2. Generate a model struct
  QString model_struct;
  for(const auto& [k, v] : g.merged_addresses)
  {
    QString structdef = "struct {\n";

    QString sanitized_dev = k;
    ossia::net::sanitize_name(sanitized_dev);

    for(const auto& vv : v)
    {
      std::string variable_name = vv.join("_").toStdString();
      ossia::net::sanitize_name(variable_name);

      std::string key = fmt::format("{}.{}", sanitized_dev.toStdString(), variable_name);
      auto it = model_field_types.find(key);
      const char* type = (it != model_field_types.end()) ? it->second.c_str() : "float";

      structdef += fmt::format(
          "struct {{ {0} value = {{}}; bool changed = false; }} {1};\n", type,
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

  std::string includes;
  if(!model_field_types.empty())
    includes += "#include <vector>\n";

  return QString::fromStdString(fmt::format(
      R"_(#pragma once
{}struct ossia_model_t
{{
    {}
}} ossia_model;
)_",
      includes, model_struct.toStdString()));
}
}
