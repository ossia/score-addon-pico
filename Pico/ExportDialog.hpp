#pragma once
#include <score/document/DocumentContext.hpp>

#include <QDialog>
class QComboBox;
class QLineEdit;
class QPlainTextEdit;
class QDialogButtonBox;

namespace Pico
{
class AppPlug;
class ExportDialog : public QDialog
{
public:
  enum ExportMode
  {
    RemoteControl,
    FullScore
  };
  AppPlug& plug;
  explicit ExportDialog(AppPlug& plug, QWidget* parent);

  bool on_export();

  void on_build();

private:
  bool copy_template_folder();
  void append_stdout(QString);
  void append_stderr(QString);

  QComboBox* m_mode{};
  QLineEdit* m_template{};
  QLineEdit* m_destination{};
  QPlainTextEdit* m_textArea{};
  QDialogButtonBox* m_buttons{};
};
bool export_device(const score::DocumentContext& ctx, QString dest);
bool export_scenario(const score::DocumentContext& ctx, QString dest);
}
