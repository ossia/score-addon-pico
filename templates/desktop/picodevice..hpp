
#include <ExprtkMapper.hpp>
#include <avnd/../../examples/Advanced/Utilities/Math.hpp>
#include <avnd/common/export.hpp>
#include <avnd/introspection/input.hpp>
#include <avnd/introspection/messages.hpp>
#include <avnd/introspection/output.hpp>
#include <avnd/introspection/port.hpp>
#include <avnd/wrappers/avnd.hpp>
#include <avnd/wrappers/controls.hpp>
#include <avnd/wrappers/metadatas.hpp>
#include <cinttypes>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>

static void ossia_task_0() {
  {
    static ao::ColorAutomation p0{
        .inputs = {.curve = {.value = {{
                                 {0.0117096018735363, {0, 0, 0}},
                                 {0.8, {100, -5.3357444, 5.433189}},
                             }}}}}; // Color automation
    static avnd_tools::PatternUnfolder p1{.inputs = {
                                              .Pattern = {0},
                                          }}; // Pattern applier
    /// INPUT READ

    /// p: Color automation
    /// p: Pattern applier
    {
      do_device_read<"">(
          (avnd::input_introspection<avnd_tools::PatternUnfolder>::field<0>(
               p1.inputs)
               .value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<avnd_tools::PatternUnfolder>::field<1>(
               p1.inputs)
               .value));
    }
    /// GRAPH PROCESSING

    /// p: Color automation
    p0({.frames = g_tick.frames,
        .relative_position = g_tick.relative_position});
    /// p: Pattern applier
    {
      value_adapt(
          (avnd::input_introspection<avnd_tools::PatternUnfolder>::field<0>(
               p1.inputs)
               .value),
          p0.outputs.value.value);
    }
    avnd_call(p1);
    /// OUTPUT WRITE

    /// p: Color automation
    { do_network_write<"OSC:/0/led/color">(p0.outputs.value.value); }
    /// p: Pattern applier
  }
}

static void ossia_task_1() {
  {
    static LFO p0{.inputs =
                      {
                          .freq = {1},
                          .ampl = {0},
                          .offset = {0.9722222},
                          .jitter = {0},
                          .phase = {0},
                          .waveform = {ao::LFO::Inputs::Waveform::enum_type(0)},
                      },
                  .setup = {.rate = 1e6}}; // LFO
    /// INPUT READ

    /// p: LFO
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<0>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<1>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<2>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<3>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<4>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<5>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<6>(p0.inputs).value));
    }
    /// GRAPH PROCESSING

    /// p: LFO
    avnd_call(p0);
    /// OUTPUT WRITE

    /// p: LFO
    {
      do_network_write<"OSC:/*/motor/*/speed">(
          (avnd::output_introspection<LFO>::field<0>(p0.outputs).value));
    }
  }
}

static void ossia_task_2() {
  {
    static LFO p0{.inputs =
                      {
                          .freq = {1},
                          .ampl = {0.5},
                          .offset = {0.5},
                          .jitter = {0},
                          .phase = {0},
                          .waveform = {ao::LFO::Inputs::Waveform::enum_type(0)},
                      },
                  .setup = {.rate = 1e6}}; // LFO.3
    /// INPUT READ

    /// p: LFO.3
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<0>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<1>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<2>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<3>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<4>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<5>(p0.inputs).value));
    }
    {
      do_device_read<"">(
          (avnd::input_introspection<LFO>::field<6>(p0.inputs).value));
    }
    /// GRAPH PROCESSING

    /// p: LFO.3
    avnd_call(p0);
    /// OUTPUT WRITE

    /// p: LFO.3
    {
      do_network_write<"OSC:/*/led/bright">(
          (avnd::output_introspection<LFO>::field<0>(p0.outputs).value));
    }
  }
}

extern "C" void run_graph(void) {
  ossia_task_0();
  ossia_task_1();
  ossia_task_2();
}
