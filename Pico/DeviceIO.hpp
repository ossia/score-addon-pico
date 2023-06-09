#pragma once

#include <QString>

#include <map>
#include <vector>

namespace Pico
{
struct DeviceIO
{
  enum Type
  {
    Variable,
    Call,
    GPIO,
    PWM,
    ADC,
  } type{};
  enum Direction
  {
    Input,
    Output
  } direction{}; // relative to the board: a LED is likely output

  int pin{};

  std::map<QString, QString> properties;

  QString printInitialization() const;
};
}
