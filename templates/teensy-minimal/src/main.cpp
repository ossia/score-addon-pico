#if __has_include(<Audio.h>)
#include "PolySynth.hpp"
#endif
#include "ossia_embedded_api.hpp"
#include "pico.ossia-device.generated.hpp"
#include "pico.ossia-graph.generated.hpp"

#if __has_include(<Audio.h>)
#include <Audio.h>
#endif

#include <Arduino.h>
void setup()
{
  ossia_init_board();
}

extern "C" int _write(...) { }
void loop()
{
  ossia_read_pins();

  ossia_run_graph();

  ossia_write_pins();

  delayMicroseconds(1000);
}
