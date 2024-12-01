#include "ExportDialog.hpp"

#include <Explorer/DocumentPlugin/DeviceDocumentPlugin.hpp>

#include <Scenario/Document/BaseScenario/BaseScenario.hpp>
#include <Scenario/Document/ScenarioDocument/ScenarioDocumentModel.hpp>

#include <QApplication>
#include <QComboBox>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDirIterator>
#include <QFileInfo>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

#include <Pico/ApplicationPlugin.hpp>
#include <Pico/Pruner.hpp>
#include <Pico/SourcePrinter.hpp>

#define OSSIA_PROTOCOL_SIMPLEIO 1
#include <Protocols/SimpleIO/CodeWriter/ESP32.hpp>
#include <Protocols/SimpleIO/SimpleIODevice.hpp>
namespace Pico
{

ExportDialog::ExportDialog(AppPlug& plug, QWidget* parent)
    : QDialog{parent}
    , plug{plug}
{
  auto lay = new QFormLayout{this};
  /// Export mode
  m_mode = new QComboBox{this};
  m_mode->addItems({tr("Remote-control"), tr("Full score")});
  lay->addRow(tr("Mode"), m_mode);

  m_template = new QLineEdit{this};
  m_template->setText(
      "/home/jcelerier/projets/oss/puara-module-template/ossia-device-test");
  lay->addRow(tr("Path to template"), m_template);

  m_destination = new QLineEdit{this};
  m_destination->setText("/tmp/test");
  lay->addRow(tr("Destination"), m_destination);

  m_textArea = new QPlainTextEdit{this};
  m_textArea->setTextInteractionFlags(Qt::TextInteractionFlag::TextBrowserInteraction);
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

  // Template
  // e.g. a puara module template ?
  // or a wasm one ?
  // or a desktop one

  // Target ?
  // note : do not overwrite copied files
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

  QDir src_dir{src};
  src_dir.setFilter(QDir::Filter::Files | QDir::Filter::NoDotAndDotDot);

  QDirIterator d{src_dir, QDirIterator::IteratorFlag::Subdirectories};

  while(d.hasNext())
  {
    auto src_file = d.next();
    auto src_relpath = src_dir.relativeFilePath(src_file);

    QString dst_file = dst + QDir::separator() + src_relpath;
    if(!QFileInfo::exists(dst_file))
    {
      QFile::copy(src_file, dst_file);
    }
  }
  return true;
}

void ExportDialog::export_device(const score::DocumentContext& ctx)
{
  auto& devices = ctx.plugin<Explorer::DeviceDocumentPlugin>();
  for(auto dev : devices.list().devices())
  {
    if(auto obj = qobject_cast<Protocols::SimpleIODevice*>(dev))
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

      QFile dst{
          m_destination->text() + QDir::separator() + "ossia.device.generated.cpp"};
      dst.open(QIODevice::WriteOnly);
      dst.write(ret.data(), ret.size());
      dst.flush();

      break;
    }
  }
}

void ExportDialog::export_scenario(const score::DocumentContext& ctx)
{
  Scenario::ScenarioDocumentModel& base
      = score::IDocument::get<Scenario::ScenarioDocumentModel>(ctx.document);

  const auto& baseInterval = base.baseScenario().interval();
  if(baseInterval.processes.size() == 0)
    return;

  auto root = m_destination->text() + QDir::separator();

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
}

void ExportDialog::on_export()
{
  auto doc = plug.currentDocument();
  if(!doc)
    return;

  if(!copy_template_folder())
  {
    qDebug() << "Failed to copy template folder from " << m_template->text() << " to "
             << m_destination->text();
    return;
  }

  switch(static_cast<ExportMode>(m_mode->currentIndex()))
  {
    case RemoteControl:
      export_device(doc->context());
      break;
    case FullScore:
      export_scenario(doc->context());
      break;
  }
}

void ExportDialog::on_build()
{
  on_export();
}

}
