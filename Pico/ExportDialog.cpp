#include "ExportDialog.hpp"

#include <Device/Protocol/DeviceInterface.hpp>

#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>

#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>

#include <ossia/network/base/device.hpp>
#include <ossia/network/base/node.hpp>

#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QProcess>
#include <QPushButton>

#include <Pico/ApplicationPlugin.hpp>
#include <Pico/OSCQueryGenerator.hpp>
#include <Pico/Pruner.hpp>
#include <Pico/SourcePrinter.hpp>

#define OSSIA_PROTOCOL_SIMPLEIO 1
#include <Protocols/SimpleIO/CodeWriter/ESP32.hpp>
#include <Protocols/SimpleIO/SimpleIODevice.hpp>
namespace Pico
{
static void copy_folder_recursively(const QString& src, const QString& dst)
{
  QDirIterator it{
      src, QDir::Filter::NoDotAndDotDot | QDir::Filter::Files | QDir::Filter::Dirs,
      QDirIterator::Subdirectories};
  QDir dir{src};
  const int absSourcePathLength = dir.absoluteFilePath(src).length();

  while(it.hasNext())
  {
    const auto fi = it.nextFileInfo();
    const QString abs_path = dst + fi.absoluteFilePath().mid(absSourcePathLength);

    if(fi.isDir())
      QDir{}.mkpath(abs_path);
    else if(fi.isFile() && !QFileInfo::exists(abs_path))
      QFile::copy(fi.absoluteFilePath(), abs_path);
  }
}

ExportDialog::ExportDialog(AppPlug& plug, QWidget* parent)
    : QDialog{parent}
    , plug{plug}
{
  auto lay = new QFormLayout{this};
  m_mode = new QComboBox{this};
  m_mode->addItems({tr("Remote-control"), tr("Full score")});
  lay->addRow(tr("Mode"), m_mode);

  m_template = new QLineEdit{this};
  m_template->setText(PICO_SOURCE_DIR "/templates/totem-tinypico-firmware");
  lay->addRow(tr("Path to template"), m_template);

  m_destination = new QLineEdit{this};
  m_destination->setText("/tmp/test");
  lay->addRow(tr("Destination"), m_destination);

  m_textArea = new QPlainTextEdit{this};
  // QTBUG-131762
  // m_textArea->setTextInteractionFlags(Qt::TextInteractionFlag::TextBrowserInteraction);
  lay->addWidget(m_textArea);

  m_buttons = new QDialogButtonBox{this};
  connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
  connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

  m_buttons->addButton(QDialogButtonBox::StandardButton::Close);
  auto export_btn = m_buttons->addButton("Export", QDialogButtonBox::ActionRole);
  connect(export_btn, &QPushButton::clicked, this, &ExportDialog::on_export);
  auto build_btn = m_buttons->addButton("Build", QDialogButtonBox::ActionRole);
  connect(build_btn, &QPushButton::clicked, this, &ExportDialog::on_build);

  lay->addWidget(m_buttons);
}

bool ExportDialog::copy_template_folder()
{
  auto src = m_template->text();
  if(!QFileInfo{src}.isDir())
    return false;

  auto dst = m_destination->text();
  if(!QFileInfo{dst}.isDir())
  {
    if(!QDir{}.mkpath(dst))
      return false;
  }

  copy_folder_recursively(src, dst);
  return true;
}

bool ExportDialog::export_device(const score::DocumentContext& ctx)
{
  // For template ossia-device
  auto& devices = ctx.plugin<Explorer::DeviceDocumentPlugin>();
  for(auto dev : devices.list().devices())
  {
    if(auto obj = qobject_cast<Protocols::SimpleIODevice*>(dev))
    {
      // Write the device OSC UDP I/O
      {
        Protocols::SimpleIOCodeWriter_ESP32 wr{*obj};
        std::string ret;
        ret += R"_(
#include "ossia_embedded_api.hpp"
#include "constants.hpp"
#include "utility.hpp"

#include <soc/adc_channel.h>
)_";
        ret += wr.init();
        ret += wr.readOSC();
        ret += wr.readPins();
        ret += wr.writeOSC();
        ret += wr.writePins();

        {
          QFile dst{
              m_destination->text() + QDir::separator() + "ossia-device.generated.cpp"};
          dst.open(QIODevice::WriteOnly);
          dst.write(ret.data(), ret.size());
          dst.flush();
        }
      }

      // Write the device OSCQuery API
      {
        auto root_model_node = obj->getNode(State::Address{});
        root_model_node.set(obj->settings());
        auto ret = Pico::oscquery_generate_namespace(root_model_node);
        {
          QFile dst{
              m_destination->text() + QDir::separator()
              + "ossia-oscquery.generated.hpp"};
          dst.open(QIODevice::WriteOnly);
          dst.write(ret.toUtf8());
          dst.flush();
        }
      }

      return true;
    }
  }
  return false;
}

bool ExportDialog::export_scenario(const score::DocumentContext& ctx)
{
  // For template ossia-full-*
  Scenario::ScenarioDocumentModel& base
      = score::IDocument::get<Scenario::ScenarioDocumentModel>(ctx.document);

  const auto& baseInterval = base.baseScenario().interval();
  if(baseInterval.processes.size() == 0)
    return false;

  QString root = m_destination->text() + QDir::separator();

  ProcessScenario ps{ctx};
  {
    QString scenario = ps.process(baseInterval);
    QFile f(root + "ossia-scenario.generated.cpp");
    f.open(QIODevice::WriteOnly);
    f.write(scenario.toUtf8());
  }

  IntervalSplit is{ctx};
  {
    auto task_text = is.process(baseInterval);

    QFile f(root + "ossia-tasks.generated.cpp");
    f.open(QIODevice::WriteOnly);
    f.write(task_text.toUtf8());
  }

  ComponentBasedSplit p{ctx};
  {
    auto res = p.process(baseInterval);
    for(const auto& [devname, content] : res)
    {
      QFile f(root + "ossia-component." + devname + ".generated.cpp");
      f.open(QIODevice::WriteOnly);
      f.write(content.toUtf8());
    }
  }
  return true;
}

bool ExportDialog::on_export()
{
  auto doc = plug.currentDocument();
  if(!doc)
    return false;

  if(!copy_template_folder())
  {
    qDebug() << "Failed to copy template folder from " << m_template->text() << " to "
             << m_destination->text();
    return false;
  }

  switch(static_cast<ExportMode>(m_mode->currentIndex()))
  {
    case RemoteControl:
      return export_device(doc->context());
    case FullScore:
      return export_scenario(doc->context());
  }
  return false;
}

void ExportDialog::append_stdout(QString s)
{
  this->m_textArea->appendPlainText(s);
}

void ExportDialog::append_stderr(QString s)
{
  this->m_textArea->appendPlainText(s);
}

void ExportDialog::on_build()
{
  if(!on_export())
    return;

  QString root = m_destination->text() + QDir::separator();
  QProcess* p = new QProcess;
  p->setWorkingDirectory(root);
  connect(p, &QProcess::readyReadStandardOutput, this, [this, p]() {
    this->append_stdout(p->readAllStandardOutput());
  });
  connect(p, &QProcess::readyReadStandardError, this, [this, p]() {
    this->append_stderr(p->readAllStandardError());
  });
  connect(p, &QProcess::finished, p, &QProcess::deleteLater);

  if(QFile::exists(root + "platformio.ini"))
  {
    p->start(
        qEnvironmentVariable("HOME") + "/.platformio/penv/bin/pio",
        {"run", "-t", "upload"});
  }
  else if(QFile::exists(root + "build.sh"))
  {
    p->start(root + "build.sh");
  }
  else if(QFile::exists(root + "CMakeLists.txt"))
  {
    p->start("cninja");
  }
  else
  {
    delete p;
  }
}

}
