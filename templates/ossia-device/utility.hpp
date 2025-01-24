#pragma once
#include <OSCBundle.h>
#include <OSCMessage.h>

// To display boot text
struct Initialize {
  bool m_error = false;
  explicit Initialize(const char* arg) {
    Serial.print("Initialize ");
    Serial.print(arg);    
  }

  void error() { m_error = true; }

  ~Initialize()
  {
    if(m_error)
      Serial.println(" ... KO!");
    else
      Serial.println(" ... OK!");
  }
};


float get_float(OSCMessage& msg)
{
  if(msg.isFloat(0))
    return msg.getFloat(0);
  else if(msg.isInt(0))
    return msg.getInt(0);
  else if(msg.isBoolean(0))
    return msg.getBoolean(0) ? 1.f : 0.f;
  else
    return -10.;
}