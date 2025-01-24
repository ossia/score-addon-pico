#include "PolySynth.hpp"
#include "ossia_embedded_api.hpp"
#include "pico.ossia-graph.generated.hpp"

#include <Audio.h>

#include <Arduino.h>
void setup()
{
  ossia_init_board();
}

void loop()
{
  ossia_read_pins();
  ossia_read_net();

  ossia_run_graph();

  ossia_write_pins();
  ossia_write_net();

  delay(100);
}
