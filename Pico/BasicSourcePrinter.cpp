#include "BasicSourcePrinter.hpp"

#include <Process/CodeWriter.hpp>
#include <Process/Dataflow/Cable.hpp>
#include <Process/Dataflow/Port.hpp>
#include <Process/Process.hpp>

#include <ossia/detail/algorithms.hpp>
#include <ossia/detail/fmt.hpp>

#include <Pico/LibossiaGenerator.hpp>
#include <Pico/SourcePrinter.hpp>

namespace Pico
{
static const std::string error_string = ":*:ERROR:*:";
static const std::string todo_string = ":*:TODO:*:";
static std::string address_to_device_read_index(const State::Address& addr)
{
  return fmt::format("do_device_read(\"{}\")", addr.toString().toStdString());
}

static std::string address_to_network_read_index(const State::Address& addr)
{
  return fmt::format("do_network_read(\"{}\")", addr.toString().toStdString());
}

static std::string
address_to_device_write_index(const State::Address& addr, std::string var)
{
  return fmt::format(
      "do_device_write(\"{}\", {})", addr.toString().toStdString(), var);
}

static std::string address_to_network_write_index(const State::Address& addr)
{
  return fmt::format(
      "do_network_write(\"{}\")", addr.toString().toStdString());
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

QString BasicSourcePrinter::print(
    const Device& device,
    const score::DocumentContext& context,
    const GraphTasks& components)
{
  // 4. Print source code
  std::string source;
  source += Pico::defaultIncludes();

  std::string task_creation;
  int task_n = 0;

  qDebug() << " <<<<>>>> there are " << components.size() << " components ";
  ;
  for (auto& comp : components)
  {
    source += "\n";
    source += fmt::format("static void ossia_task_{}(void* arg) {{\n", task_n);
    source += printTask(device, context, comp).toStdString();
    source += "}\n";

    task_creation += fmt::format(R"_(ossia_task_{0}();)_", task_n);
    task_creation += "\n";
    task_n++;
  }

  // Write the main:

  source += fmt::format(
      R"_(
extern "C" void run_graph(void)
{{
  {}
}}
)_",
      task_creation);

  return QString::fromStdString(source);
}
QString BasicSourcePrinter::printDeviceInitialization(const Device& device)
{
  QString str;

  return str;
}
}
