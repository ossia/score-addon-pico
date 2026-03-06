#if __has_include(<Audio.h>)
#include "PolySynth.hpp"
#endif
#include "ossia_embedded_api.hpp"
#include "pico.ossia-graph.generated.hpp"

#if __has_include(<Audio.h>)
#include <Audio.h>
#endif

#include <Arduino.h>
void setup()
{
  // ossia_init_board();
}

extern "C" int _write(...) { }
void loop()
{
  // note: enabling Serial makes time very slow when the serial monitor is not connected
  // Serial.begin(115200);
  // ossia_read_pins();
  // ossia_read_net();

  ossia_run_graph();

  // ossia_write_pins();
  // ossia_write_net();

  delayMicroseconds(1000);
}
