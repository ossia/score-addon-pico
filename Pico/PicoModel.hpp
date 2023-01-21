#pragma once

#include <halp/audio.hpp>
#include <halp/controls.hpp>
#include <halp/meta.hpp>

namespace Pico
{

class Pico
{
public:
  halp_meta(name, "My Avendish Gain")
  halp_meta(category, "Audio")
  halp_meta(c_name, "my_gain")
  halp_meta(uuid, "304a52f4-d6e8-4493-aa10-f16d50e5e60d")

  // Define inputs and outputs ports.
  // See the docs at https://github.com/celtera/avendish
  struct ins
  {
    halp::dynamic_audio_bus<"Input", double> audio;
    halp::knob_f32<"Weight", halp::range{.min = 0., .max = 10., .init = 0.5}> gain;
  } inputs;

  struct
  {
    halp::dynamic_audio_bus<"Output", double> audio;
  } outputs;

  using setup = halp::setup;
  void prepare(halp::setup info)
  {
    // Initialization, this method will be called with buffer size, etc.
  }

  // Do our processing for N samples
  using tick = halp::tick;

  // Defined in the .cpp
  void operator()(halp::tick t);

  // UI is defined in another file to keep things clear.
  struct ui;
};

}
