#include "ESPSourcePrinter.hpp"

#include <Process/CodeWriter.hpp>
#include <Process/Dataflow/Cable.hpp>
#include <Process/Dataflow/Port.hpp>
#include <Process/Process.hpp>

#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/fmt.hpp>

#include <Pico/SourcePrinter.hpp>

namespace Pico
{
static const std::string error_string = ":*:ERROR:*:";
static const std::string todo_string = ":*:TODO:*:";
static std::string address_to_device_read_index(const State::Address& addr)
{
  if (addr.path.size() < 2)
    return error_string;

  // Format:
  // /gpio/1
  // /adc/2
  bool ok{};
  int num = addr.path[1].toInt(&ok);
  if (!ok)
    return error_string;
  if (addr.path.startsWith("gpio"))
  {
    return fmt::format("gpio_get_level(GPIO_NUM_{})", num);
  }
  else if (addr.path.startsWith("adc"))
  {
    return fmt::format("adc1_get_raw(ADC1_CHANNEL_{})", num);
  }
  return todo_string;
}

static std::string address_to_network_read_index(const State::Address& addr)
{
  return todo_string;
}

static std::string
address_to_device_write_index(const State::Address& addr, std::string var)
{
  if (addr.path.size() < 2)
    return error_string;

  bool ok{};
  int num = addr.path[1].toInt(&ok);
  if (!ok)
    return error_string;
  if (addr.path.startsWith("gpio"))
  {
    return fmt::format("gpio_set_level(GPIO_NUM_{}, {})", num, var);
  }
  else if (addr.path.startsWith("pwm"))
  {
    return fmt::format(
        R"_(
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_{0}, {1});
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_{0});
)_",
        num,
        var);
  }

  return todo_string;
}

static std::string address_to_network_write_index(const State::Address& addr)
{
  return todo_string;
}

QString ESPSourcePrinter::printTask(
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
      qFatal() << "Process:" << p->metadata().getName() << typeid(*p).name()
               << " does not have a code writer";
    }
  }

  // 1. Instantiate the processes
  {
    int index = 0;
    for (auto& wr : codes)
    {
      std::string name = procs[index]->metadata().getName().toStdString();
      wr->variable = fmt::format("p{}", index);
      c += fmt::format("{} {}; // {}\n", wr->typeName(), wr->variable, name);
      index++;
    }
  }

  // Start the loop
  c += "for(;;) { \n";

  // 2. Write the inputs addresses
  {
    int process_index = 0;
    for (auto* p : procs)
    {
      auto& wr = codes[process_index];

      for (auto inl : p->inlets())
      {
        if (inl->address().address.device == device.name)
        {
          c += fmt::format(
              "{{ {} = {}; }}\n",
              wr->accessInlet(inl->id()),
              address_to_device_read_index(inl->address().address));
        }
        else if (!inl->address().address.device.isEmpty())
        {
          c += fmt::format(
              "{{ {} = world.read_network<{}>(); }}\n",
              wr->accessInlet(inl->id()),
              address_to_network_read_index(inl->address().address));
        }
      }

      process_index++;
    }
  }

  // 3. Write down the graph
  {
    int process_index = 0;
    for (auto& wr : codes)
    {
      auto& p = *procs[process_index];
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
                "{{ {} = {}; }}\n",
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

  // 4. Write the outputsaddresses
  {
    int process_index = 0;
    for (auto* p : procs)
    {
      auto& wr = codes[process_index];

      for (auto inl : p->outlets())
      {
        if (inl->address().address.device == device.name)
        {
          c += fmt::format(
              "{{ {}; }}\n",
              address_to_device_write_index(
                  inl->address().address, wr->accessOutlet(inl->id())));
        }
        else if (!inl->address().address.device.isEmpty())
        {
          c += fmt::format(
              "{{ world.write_network<{}>({}); }}\n",
              address_to_network_write_index(inl->address().address),
              wr->accessOutlet(inl->id()));
        }
      }

      process_index++;
    }
  }

  // End the loop
  c += "}\n}\n";

  return QString::fromStdString(c);
}

QString ESPSourcePrinter::print(
    const Device& device,
    const score::DocumentContext& context,
    const GraphTasks& components)
{
  // 4. Print source code
  std::string source;
  source += Pico::defaultIncludesGraph();
  source += R"_(


#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

#include <driver/adc.h>
#include <driver/gpio.h>
#include <driver/ledc.h>

#include <esp_err.h>

struct world {
  template <std::size_t N> auto read_local() {
    return gpio_get_level((gpio_num_t)0);
  }
  template <std::size_t N> auto write_local(auto v) {
    gpio_set_level((gpio_num_t)N, v);
  }
} world;



)_";

  std::string task_creation;
  int task_n = 0;

  qDebug() << " <<<<>>>> there are " << components.size() << " components ";
  ;
  for (auto& comp : components)
  {
    source += "\n";
    source += fmt::format("static void ossia_task_{}(void* arg) {{\n", task_n);
    source += printTask(device, context, comp.processes).toStdString();
    source += "}\n";

    task_creation += fmt::format(
        R"_(xTaskCreate(ossia_task_{0}, "ossia_task_{0}", 4096, nullptr, 10, nullptr);)_",
        task_n);
    task_creation += "\n";
    task_n++;
  }

  // Write the main:

  source += fmt::format(
      R"_(
extern "C" void app_main(void)
{{
   // 1. Initialize all the things we need from the device
  {{
    {}
  }}

  // 2. Create all the tasks
  {{
    {}
  }}

   // 3. lesgo
   for(;;) vTaskDelay(1000);
}}
)_",
      printDeviceInitialization(device).toStdString(),
      task_creation);

  qDebug() << source.c_str();
  return QString::fromStdString(source);

  // - First list all the used processes
  // => for now we'll just include them all in the header

  // - Abstract the input reading / output writing
  // => see world.read_/write_

  // - Faust code? https://faustdoc.grame.fr/tutorials/esp32/
  // - ExprTK code? try to compile it to C directly,
  // - Reduction step for basic processes? e.g. addition, etc

#if 0
  // Let's assume we want to do gpio2 = sin(gpio0 + gpio1)
  float gpio0{}, gpio1{}, gpio2{};
  world w;
  avnd::blocks::Addition n0;
  avnd::blocks::Sin n1;

  // Addresses in:
  avnd::parameter_input_introspection<decltype(n0)>::get<0>(n0.inputs).value
      = w.read_local<0>();
  avnd::parameter_input_introspection<decltype(n0)>::get<1>(n0.inputs).value
      = w.read_local<1>();

  n0();

  avnd::parameter_input_introspection<decltype(n1)>::get<0>(n1.inputs).value
      = avnd::parameter_output_introspection<decltype(n0)>::get<0>(n0.outputs)
            .value;
  n1();

  // Addresses out
  w.write_local<2>(
      avnd::parameter_output_introspection<decltype(n1)>::get<0>(n1.outputs)
          .value);
#endif
}
QString ESPSourcePrinter::printDeviceInitialization(const Device& device)
{
  QString str;

  for (auto& ios : device.ios)
  {
    if (ios.direction == ios.Input)
    {
      str += "{ \n" + ios.printInitialization() + "\n }\n";
    }
  }
  for (auto& ios : device.ios)
  {
    if (ios.direction == ios.Output)
    {
      str += "{ \n" + ios.printInitialization() + "\n }\n";
    }
  }

  return str;
}
}
