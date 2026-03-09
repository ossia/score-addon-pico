#pragma once
#include "ExprtkMapper.hpp"

#include <Fx/AngleNode.hpp>
#include <Fx/Arpeggiator.hpp>
#include <Fx/Chord.hpp>
#include <Fx/ClassicalBeat.hpp>
// #include <Fx/EmptyMapping.hpp>
// #include <Fx/Envelope.hpp>
// #include <Fx/LFO.hpp>
#include <Fx/LFO_v2.hpp>
/// #include <Fx/Looper.hpp>
#include <Fx/Metro.hpp>
#include <Fx/MidiHiRes.hpp>
// FIXME #include <Fx/MidiToArray.hpp>
#include <Fx/MidiUtil.hpp>
#include <Fx/NoiseFilter.hpp>
#include <Fx/PitchToValue.hpp>
#include <Fx/Quantifier.hpp>
#include <Fx/RateLimiter.hpp>
/// #include <Fx/Smooth.hpp>
#include <Fx/Smooth_v2.hpp>
/// #include <Fx/Types.hpp>
// #include <Fx/VelToNote.hpp>
// #include <Fx/VelToNote_mono.hpp>
// BAD #include <Midi/Arpeggiator.hpp>
#include <Midi/MidiGlide.hpp>
#include <Midi/MidiHiResInput.hpp>
#include <Midi/MidiHiResOutput.hpp>
#include <Midi/MidiPitch.hpp>
#include <Midi/MidiSyncInput.hpp>

#include <ossia/network/value/value.hpp>

// #include <Advanced/AI/PromptComposer.hpp>
// #include <Advanced/AI/PromptInterpolator.hpp>
// #include <Advanced/Audio/AudioFilters.hpp>
/// #include <Advanced/Audio/AudioSum.hpp>
// #include <Advanced/Audio/Bitcrush.hpp>
// #include <Advanced/Audio/Convolver.hpp>
// #include <Advanced/Audio/Dynamics.hpp>
// #include <Advanced/Audio/Echo.hpp>
// #include <Advanced/Audio/Flanger.hpp>
// #include <Advanced/Audio/Gain.hpp>
// #include <Advanced/Audio/MonoMix.hpp>
// #include <Advanced/Audio/QuadPan.hpp>
// #include <Advanced/Audio/Silence.hpp>
// #include <Advanced/Audio/Soundfile.hpp>
// #include <Advanced/Audio/Splitter.hpp>
// #include <Advanced/Audio/StereoMixer.hpp>
//#include <Advanced/AudioParticles/AudioParticles.hpp>
// #include <Advanced/AudioStream/AudioReceiver.hpp>
// #include <Advanced/AudioStream/AudioSender.hpp>
// #include <Advanced/AudioStream/Config.hpp>
// #include <Advanced/Display/ColorDisplay.hpp>
/// #include <Advanced/GeoZones/GeoZones.hpp>
/// #include <Advanced/MidiScaler/MidiFilter.hpp>
// #include <Advanced/MidiScaler/MidiReader.hpp>
// #include <Advanced/MidiScaler/MidiScaler.hpp>
// #include <Advanced/MidiScaler/MidiScroller.hpp>
// #include <Advanced/MidiScaler/StrStream.hpp>
/// #include <Advanced/Patternal/Melodial.hpp>
/// #include <Advanced/Patternal/Patternal.hpp>
/// #include <Advanced/Synth/Wavecycle.hpp>
// #include <Advanced/UI/2DView.hpp>
// #include <Advanced/UI/Knob.hpp>
// #include <Advanced/UI/LEDView.hpp>
// #include <Advanced/UI/Multislider.hpp>
// #include <Advanced/UI/PulseView.hpp>
#include <Advanced/UI/Widgets.hpp>
// #include <Advanced/Utilities/ADSR.hpp>
// #include <Advanced/Utilities/ArrayCombiner.hpp>
#include <Advanced/Utilities/Combine.hpp>
/// #include <Advanced/Utilities/LFO.hpp>

///// SIZE HOG #include <Advanced/Utilities/Accumulator.hpp>
///// SIZE HOG #include <Advanced/Utilities/Calibrator.hpp>
#include <Advanced/Utilities/ArrayFlattener.hpp>
#include <Advanced/Utilities/ArrayRecombiner.hpp>
#include <Advanced/Utilities/ArrayTool.hpp>
#include <Advanced/Utilities/Automation.hpp>
#include <Advanced/Utilities/ColorAutomation.hpp>
#include <Advanced/Utilities/ControlToCV.hpp>
/////
#include <Advanced/Utilities/Counter.hpp>
#include <Advanced/Utilities/Logic.hpp>
#include <Advanced/Utilities/MapTool.hpp>
#include <Advanced/Utilities/Mapping.hpp>
#include <Advanced/Utilities/Math.hpp>
#include <Advanced/Utilities/MultiChoice.hpp>
#include <Advanced/Utilities/RangeFilter.hpp>
/////
#include <Advanced/Utilities/RangeMapper.hpp>
#include <Advanced/Utilities/RepetitionFilter.hpp>
#include <Advanced/Utilities/Smooth.hpp>
#include <Advanced/Utilities/Spread.hpp>
// #include <Advanced/Utilities/TimecodeSynchronizer.hpp>
#include <Advanced/Utilities/Trigger.hpp>
#include <Advanced/Utilities/Tweener.hpp>
#include <Advanced/Utilities/ValueFilter.hpp>
#include <Advanced/Utilities/ValueMixer.hpp>
#include <Helpers/GradientScrubber.hpp>
// #include <Helpers/Lowpass.hpp>
#include <Helpers/Noise.hpp>
#include <Helpers/Peak.hpp>
#include <Helpers/Poles.hpp>
#include <Helpers/Sines.hpp>
#include <Helpers/SmoothGain.hpp>

//#include <Helpers/ValueDelay.hpp>
// #include <Ports/Essentia/essentia.hpp>
// #include <Ports/Essentia/stats/Entropy.hpp>
// #include <Ports/LitterPower/CCC.hpp>
//#include <Ports/WaveDigitalFilters/PassiveLPF.hpp>
//#include <Ports/WaveDigitalFilters/VoltageDivider.hpp>
#include <Puara/Scaler.hpp>
// #include <Raw/Addition.hpp>
// #include <Raw/Aggregate.hpp>
// #include <Raw/AllPortsTypes.hpp>
// #include <Raw/Callback.hpp>
// #include <Raw/Construct.hpp>
// #include <Raw/DoubleArray.hpp>
// #include <Raw/DynamicPort.hpp>
// #include <Raw/Init.hpp>
#include <Raw/Interpolator.hpp>
// #include <Raw/Lowpass.hpp>
// #include <Raw/Messages.hpp>
// #include <Raw/Midi.hpp>
// #include <Raw/MiniPerSample.hpp>
// #include <Raw/Minimal.hpp>
// #include <Raw/Modular.hpp>
// #include <Raw/Optional.hpp>
// #include <Raw/PerSampleProcessor.hpp>
// #include <Raw/PerSampleProcessor2.hpp>
// #include <Raw/Presets.hpp>
// #include <Raw/ProcessLauncher.hpp>
// #include <Raw/Processor.hpp>
// #include <Raw/Random.hpp>
// #include <Raw/Reactive.hpp>
// #include <Raw/SampleAccurateControls.hpp>
// #include <Raw/Shell.hpp>
// #include <Raw/Sines.hpp>
// #include <Raw/SpanControls.hpp>
// #include <Raw/TimingSplitter.hpp>
// #include <Raw/Ui.hpp>
// #include <Raw/Variant.hpp>
#include <avnd/common/export.hpp>
#include <avnd/common/for_nth.hpp>
#include <avnd/introspection/input.hpp>
#include <avnd/introspection/messages.hpp>
#include <avnd/introspection/output.hpp>
#include <avnd/introspection/port.hpp>
#include <avnd/wrappers/avnd.hpp>
#include <avnd/wrappers/controls.hpp>
#include <avnd/wrappers/metadatas.hpp>
#include <halp/audio.hpp>

#include <Arduino.h>
#include <cmath>

#include <array>
#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <tuple>

// #include <Ports/VB/vb.fourses_tilde.hpp>
#include <avnd/../../examples/Advanced/Utilities/Math.hpp>
static constexpr int sample_rate = 44100;
static constexpr halp::setup g_setup{
    .input_channels = 0, .output_channels = 0, .frames = 128, .rate = sample_rate};

namespace ossia
{
inline value::~value() noexcept = default;
}
struct g_tick_t
{
  int64_t tick_micros{};
  int frames{};
  float relative_position{};

  unsigned long prev_micros{};
  bool initialized{};

  int64_t total_frames{};
  int64_t total_micros{};
  void update_timings()
  {
    auto t = micros();
    if(!initialized)
    {
      // First call: just record the time, don't compute a huge delta
      prev_micros = t;
      tick_micros = 0;
      frames = 0;
      initialized = true;
      return;
    }

    if(prev_micros > t)
    {
      // micros() overflow (wraps every ~70 minutes on 32-bit)
      auto remaining_prev = std::numeric_limits<unsigned long>::max() - prev_micros;
      auto total_time = remaining_prev + t;

      tick_micros = total_time;
    }
    else
    {
      tick_micros = t - prev_micros;
    }
    prev_micros = t;

    frames = sample_rate * tick_micros / 1000000;
    total_micros += tick_micros;
    total_frames += frames;
  }
} g_tick;

static int get_frames(const g_tick_t& tk)
{
  return tk.frames;
}
static float get_relative_position(const g_tick_t& tk)
{
  return tk.relative_position;
}

static constexpr void avnd_clean_inputs(auto& proc)
{
  avnd::for_each_field_ref(proc.inputs, [](auto& p) {
    if constexpr(requires { p.value = std::nullopt; })
      p.value = std::nullopt;
  });
}

static constexpr void avnd_clean_outputs(auto& proc)
{
  avnd::for_each_field_ref(proc.outputs, [](auto& p) {
    if constexpr(requires { p.value = std::nullopt; })
    {
      p.value = std::nullopt;
    }
  });
}

template <typename T>
static constexpr void avnd_call(T& proc)
{
  if constexpr(requires { proc(g_tick.frames); })
    proc(g_tick.frames);
  else if constexpr(requires { proc(); })
    proc();
  else
  {
    using tick_type = typename T::tick;
    tick_type tk;
    tk.frames = g_tick.frames;

    // Assumed fixed musical parameters
    constexpr double tempo = 120.;
    constexpr int sig_num = 4;
    constexpr int sig_denom = 4;
    // Quarter notes per second at this tempo
    constexpr double quarters_per_second = tempo / 60.;
    // Quarter notes per bar in this time signature (4/4 = 4 quarters per bar)
    constexpr double quarters_per_bar = 4.0 * sig_num / sig_denom;
    // 705,600,000 flicks per second -> 705.6 flicks per microsecond
    constexpr double flicks_per_micro = 705.6;

    // total_micros already includes tick_micros after update_timings()
    // so the start of this tick is (total_micros - tick_micros)
    int64_t start_micros = g_tick.total_micros - g_tick.tick_micros;
    int64_t end_micros = g_tick.total_micros;
    double start_seconds = start_micros / 1e6;
    double end_seconds = end_micros / 1e6;

    if constexpr(requires { tk.position_in_frames; })
      tk.position_in_frames = g_tick.total_frames - g_tick.frames;
    if constexpr(requires { tk.position_in_nanoseconds; })
      tk.position_in_nanoseconds = start_micros * 1e3;

    // Flicks timing
    if constexpr(requires { tk.start_in_flicks; })
      tk.start_in_flicks = start_micros * flicks_per_micro;
    if constexpr(requires { tk.end_in_flicks; })
      tk.end_in_flicks = end_micros * flicks_per_micro;
    if constexpr(requires { tk.relative_position; })
      tk.relative_position = 0.;
    if constexpr(requires { tk.parent_duration; })
      tk.parent_duration = 0;

    if constexpr(requires { tk.speed; })
      tk.speed = 1.;
    if constexpr(requires { tk.tempo; })
      tk.tempo = tempo;

    // Time signature
    if constexpr(requires { tk.signature; })
      tk.signature = {sig_num, sig_denom};

    // Musical positions in quarter notes
    if constexpr(requires { tk.start_position_in_quarters; })
    {
      double start_quarters = start_seconds * quarters_per_second;
      double end_quarters = end_seconds * quarters_per_second;

      tk.start_position_in_quarters = start_quarters;

      if constexpr(requires { tk.end_position_in_quarters; })
        tk.end_position_in_quarters = end_quarters;

      // Bar tracking: position of last bar boundary in quarter notes
      if constexpr(requires { tk.bar_at_start; })
        tk.bar_at_start = std::floor(start_quarters / quarters_per_bar) * quarters_per_bar;
      if constexpr(requires { tk.bar_at_end; })
        tk.bar_at_end = std::floor(end_quarters / quarters_per_bar) * quarters_per_bar;

      // No signature changes ever occur
      if constexpr(requires { tk.last_signature_change; })
        tk.last_signature_change = 0.;
    }

    proc(tk);
  }
}
using namespace ao;
struct Dummy
{
  static inline int variable;
};
template <std::size_t N>
struct static_string
{
  consteval static_string(const char (&str)[N]) noexcept
  {
    for(int i = 0; i < N; i++)
    {
      value[i] = str[i];
    }
  }

  static_assert(N > 0);
  static constexpr int length = N - 1;
  char value[N];
};

// ---- ossia::value <-> primitive type conversions ----
// Must be declared BEFORE the generic template so they are found by name lookup.

// ossia::value <- primitive types
static inline void value_adapt(ossia::value& to, float from) { to = from; }
static inline void value_adapt(ossia::value& to, double from) { to = (float)from; }
static inline void value_adapt(ossia::value& to, int from) { to = (int32_t)from; }
static inline void value_adapt(ossia::value& to, bool from) { to = from; }
static inline void value_adapt(ossia::value& to, const exprtk_arithmetic& from) { to = from.v; }
static inline void value_adapt(ossia::value& to, const ossia::value& from) { to = from; }

// ossia::value -> primitive types
static inline void value_adapt(float& to, const ossia::value& from)
{
  if(auto* f = from.target<float>())
    to = *f;
  else if(auto* i = from.target<int32_t>())
    to = (float)*i;
  else if(auto* b = from.target<bool>())
    to = *b ? 1.f : 0.f;
}
static inline void value_adapt(double& to, const ossia::value& from)
{
  if(auto* f = from.target<float>())
    to = *f;
  else if(auto* i = from.target<int32_t>())
    to = (double)*i;
  else if(auto* b = from.target<bool>())
    to = *b ? 1. : 0.;
}
static inline void value_adapt(int& to, const ossia::value& from)
{
  if(auto* i = from.target<int32_t>())
    to = *i;
  else if(auto* f = from.target<float>())
    to = (int)*f;
  else if(auto* b = from.target<bool>())
    to = *b ? 1 : 0;
}
static inline void value_adapt(bool& to, const ossia::value& from)
{
  if(auto* b = from.target<bool>())
    to = *b;
  else if(auto* f = from.target<float>())
    to = *f != 0.f;
  else if(auto* i = from.target<int32_t>())
    to = *i != 0;
}
static inline void value_adapt(exprtk_arithmetic& to, const ossia::value& from)
{
  float f{};
  value_adapt(f, from);
  to.v = f;
}

// vector<float> <-> vector<ossia::value>
static inline void value_adapt(std::vector<float>& to, const std::vector<ossia::value>& from)
{
  to.resize(from.size());
  for(std::size_t i = 0; i < from.size(); i++)
    value_adapt(to[i], from[i]);
}
static inline void value_adapt(std::vector<ossia::value>& to, const std::vector<float>& from)
{
  to.resize(from.size());
  for(std::size_t i = 0; i < from.size(); i++)
    to[i] = from[i];
}

// vector<exprtk_arithmetic> <-> vector<ossia::value>
static inline void value_adapt(std::vector<exprtk_arithmetic>& to, const std::vector<ossia::value>& from)
{
  to.resize(from.size());
  for(std::size_t i = 0; i < from.size(); i++)
    value_adapt(to[i], from[i]);
}
static inline void value_adapt(std::vector<ossia::value>& to, const std::vector<exprtk_arithmetic>& from)
{
  to.resize(from.size());
  for(std::size_t i = 0; i < from.size(); i++)
    to[i] = from[i].v;
}

// ossia::value <-> std::array<float, N>
template <std::size_t N>
static inline void value_adapt(std::array<float, N>& to, const ossia::value& from)
{
  if(auto* a = from.target<std::array<float, N>>())
    to = *a;
}
template <std::size_t N>
static inline void value_adapt(ossia::value& to, const std::array<float, N>& from)
{
  to = from;
}

// ---- Generic value_adapt template ----
template <typename To, typename From>
static inline constexpr void value_adapt(To& to, const From& from)
{
  if constexpr(requires {
                 to.value;
                 from.value;
               })
  {
    if constexpr(requires { to.value = *from.value; })
    {
      if(from.value)
        to.value = *from.value;
    }
    else
    {
      value_adapt(to.value, from.value);
    }
  }
  else if constexpr(requires { from.value; })
  {
    if constexpr(requires { *from.value; }){
      if(from.value)
        value_adapt(to, *from.value);
    }
    else {
      value_adapt(to, from.value);
    }
  }
  else if constexpr(requires { to.value; })
  {
    value_adapt(to.value, from);
  }
  else
  {
    to = from;
  }

  // FIXME
  if constexpr(requires { from.value = std::nullopt; })
    from.value = std::nullopt;
}

template <typename T>
static inline constexpr void midi_adapt(T&& to, T&& from)
{
  to = std::forward<T>(from);
}
static inline void midi_adapt(halp::midi_msg& to, libremidi::message& from)
{
  to.timestamp = from.timestamp;
  to.bytes.assign(from.bytes.begin(), from.bytes.end());
}

static inline constexpr void
value_adapt(avnd::midi_port auto&& to, avnd::midi_port auto&& from)
{
  to.midi_messages.clear();
  for(auto& m : from.midi_messages)
  {
    using to_messages_type = std::remove_cvref_t<decltype(to.midi_messages)>;
    typename to_messages_type::value_type to_msg;
    midi_adapt(to_msg, m);
    to.midi_messages.push_back(std::move(to_msg));
  }
  from.midi_messages.clear(); // FIXME
}

static inline constexpr void
value_adapt(avnd::parameter_port auto&& to, avnd::parameter_port auto&& from)
{
  to.value = from.value;
}
static inline constexpr void
value_adapt(avnd::parameter_port auto& to, exprtk_arithmetic& from)
{
  to.value = from;
}
static inline constexpr void
value_adapt(exprtk_arithmetic& to, avnd::parameter_port auto& from)
{
  to = from.value;
}

// ---- Deleted overloads (safety net for callbacks) ----
static inline constexpr void value_adapt(auto&& to, avnd::view_callback auto&& from)
    = delete;
static inline constexpr void value_adapt(auto&& to, avnd::stored_callback auto&& from)
    = delete;
static inline constexpr void value_adapt(auto&& to, avnd::callback auto&& from)
    = delete;

static inline constexpr void value_adapt(avnd::view_callback auto&& to, auto&& from)
    = delete;
static inline constexpr void value_adapt(avnd::stored_callback auto&& to, auto&& from)
    = delete;
static inline constexpr void value_adapt(avnd::callback auto&& to, auto&& from)
    = delete;

// vector<exprtk_arithmetic> <-> vector<float>
static inline void value_adapt(std::vector<float>& to, std::vector<exprtk_arithmetic>& from)
{
  to.assign(from.begin(), from.end());
}
static inline void value_adapt(std::vector<float>& to, const std::vector<exprtk_arithmetic>& from)
{
  to.assign(from.begin(), from.end());
}
static inline void value_adapt(std::vector<exprtk_arithmetic>& to, std::vector<float>& from)
{
  to.assign(from.begin(), from.end());
}
static inline void value_adapt(std::vector<exprtk_arithmetic>& to, const std::vector<float>& from)
{
  to.assign(from.begin(), from.end());
}

// optional<vector<exprtk_arithmetic>> <-> optional<vector<float>>
static inline void value_adapt(
    std::optional<std::vector<float>>& to,
    std::optional<std::vector<exprtk_arithmetic>>& from)
{
  if(from)
  {
    to.emplace().assign(from->begin(), from->end());
    from = std::nullopt;
  }
}
static inline void value_adapt(
    std::optional<std::vector<exprtk_arithmetic>>& to,
    std::optional<std::vector<float>>& from)
{
  if(from)
  {
    to.emplace().assign(from->begin(), from->end());
    from = std::nullopt;
  }
}

// port with .value of vector<float> <-> vector<exprtk_arithmetic>
template <typename Port>
  requires requires(Port p) { p.value; } && (!std::is_same_v<std::remove_cvref_t<Port>, exprtk_arithmetic>)
static inline void value_adapt(Port& to, std::vector<exprtk_arithmetic>& from)
{
  if constexpr(std::is_same_v<std::remove_cvref_t<decltype(to.value)>, std::vector<float>>)
    to.value.assign(from.begin(), from.end());
  else
    to.value = from;
}

// port with .value of vector<float> <- optional<vector<exprtk_arithmetic>>
template <typename Port>
  requires requires(Port p) { p.value; } && (!std::is_same_v<std::remove_cvref_t<Port>, exprtk_arithmetic>)
static inline void value_adapt(Port& to, std::optional<std::vector<exprtk_arithmetic>>& from)
{
  if(from)
  {
    if constexpr(std::is_same_v<std::remove_cvref_t<decltype(to.value)>, std::vector<float>>)
      to.value.assign(from->begin(), from->end());
    else
      to.value = *from;
    from = std::nullopt;
  }
}
// port with .value <- optional<exprtk_arithmetic> (scalar to port)
template <typename Port>
  requires requires(Port p) { p.value; } && (!std::is_same_v<std::remove_cvref_t<Port>, exprtk_arithmetic>)
static inline void value_adapt(Port& to, std::optional<exprtk_arithmetic>& from)
{
  if(from)
  {
    if constexpr(std::is_same_v<std::remove_cvref_t<decltype(to.value)>, std::vector<float>>)
      to.value = {from->v};
    else
      to.value = float(*from);
    from = std::nullopt;
  }
}

// scalar <- port with vector .value (take first element)
template <typename Port>
  requires requires(Port p) { p.value[0]; p.value.empty(); }
static inline void value_adapt(float& to, Port& from)
{
  if(!from.value.empty())
    to = from.value[0];
}
template <typename Port>
  requires requires(Port p) { p.value[0]; p.value.empty(); }
static inline void value_adapt(exprtk_arithmetic& to, Port& from)
{
  if(!from.value.empty())
    to = from.value[0];
}

template <typename T>
static inline constexpr void value_adapt(auto& to, std::optional<T>& from)
{
  if(from)
  {
    if constexpr(requires { to.value; })
      value_adapt(to.value, *from);
    else
      to = *from;
  }
}
template <typename T>
static inline constexpr void value_adapt(auto& to, const std::optional<T>& from)
{
  if(from)
  {
    if constexpr(requires { to.value; })
      value_adapt(to.value, *from);
    else
      to = *from;
  }
}

static inline constexpr void value_adapt(auto& to, auto& from, double factor)
{
  to = from * factor;
}
template <typename T>
static inline constexpr void value_adapt(auto& to, std::optional<T>& from, double factor)
{
  if(from)
    to = *from * factor;
}

template <typename T>
static inline constexpr void
value_adapt(auto& to, const std::optional<T>& from, double factor)
{
  if(from)
    to = *from * factor;
}


static void do_prepare(auto&& arg, auto&& tk) {
  if constexpr(requires { arg.prepare(tk); })
    arg.prepare(tk);
}

// ---- Callback output support ----
// When a process has callback outputs (halp::callback, halp::timed_callback),
// we need intermediary storage variables that the callbacks write to,
// and downstream nodes read from.

struct pico_empty_t {};

namespace pico_detail
{
// Extract the function signature from a view_callback field, stripping the void* context pointer
template <typename... Args>
struct strip_void_ptr_arg;
template <typename R, typename... Args>
struct strip_void_ptr_arg<R(void*, Args...)>
{
  using type = R(Args...);
};

// Map a function signature to the type we want to store
// void()          -> bool  (impulse)
// void(T)         -> T     (single value)
// void(T1, T2...) -> std::tuple<T1, T2...>
template <typename Sig>
struct sig_to_stored_type;
template <>
struct sig_to_stored_type<void()>
{
  using type = bool;
};
template <typename T>
struct sig_to_stored_type<void(T)>
{
  using type = T;
};
template <typename T1, typename T2, typename... Rest>
struct sig_to_stored_type<void(T1, T2, Rest...)>
{
  using type = std::tuple<T1, T2, Rest...>;
};

// Strip the first argument from a function signature (for timed callbacks)
template <typename Sig>
struct strip_first_arg;
template <typename R>
struct strip_first_arg<R()>
{
  using type = R();
};
template <typename R, typename First, typename... Rest>
struct strip_first_arg<R(First, Rest...)>
{
  using type = R(Rest...);
};

// Compute what to store for a single output field
template <typename Field, bool IsCallback = avnd::callback<Field>>
struct output_storage_for
{
  using type = pico_empty_t;
};

template <typename Field>
struct output_storage_for<Field, true>
{
  // Get the message type: void(Args...) with void* stripped
  using func_ptr_t = std::decay_t<decltype(Field{}.call.function)>;
  using raw_sig = typename strip_void_ptr_arg<std::remove_pointer_t<func_ptr_t>>::type;

  // For timed callbacks, strip the timestamp (first int64_t arg)
  using effective_sig = std::conditional_t<
      avnd::tag_timestamp<Field>, typename strip_first_arg<raw_sig>::type, raw_sig>;

  using type = std::optional<typename sig_to_stored_type<effective_sig>::type>;
};

// Build the callback storage tuple for all outputs of a process
template <typename T,
          typename Seq = std::make_index_sequence<avnd::output_introspection<T>::size>>
struct callback_storage_tuple;

template <typename T, std::size_t... Is>
struct callback_storage_tuple<T, std::index_sequence<Is...>>
{
  using outputs_t = typename avnd::outputs_type<T>::type;
  using type = std::tuple<
      typename output_storage_for<
          typename avnd::output_introspection<T>::template field_type<Is>>::type...>;
};

// The callback writer function: stores callback arguments into the intermediary storage
template <bool Timed, typename StoreType, typename FuncPtrType>
struct callback_writer;

template <bool Timed, typename StoreType, typename... AllArgs>
struct callback_writer<Timed, StoreType, void (*)(void*, AllArgs...)>
{
  static void invoke(void* ctx, AllArgs... all_args)
  {
    auto& store = *static_cast<StoreType*>(ctx);
    if constexpr(!Timed)
    {
      // Non-timed: store all args
      if constexpr(sizeof...(AllArgs) == 0)
        store = true;
      else if constexpr(sizeof...(AllArgs) == 1)
        ((store = all_args), ...);
      else
        store = typename StoreType::value_type{all_args...};
    }
    else
    {
      // Timed: skip first arg (timestamp), store the rest
      auto tup = std::tie(all_args...);
      if constexpr(sizeof...(AllArgs) <= 1)
        store = true;
      else if constexpr(sizeof...(AllArgs) == 2)
        store = std::get<1>(tup);
      else
      {
        [&]<std::size_t... Is>(std::index_sequence<Is...>) {
          store = typename StoreType::value_type{std::get<Is + 1>(tup)...};
        }(std::make_index_sequence<sizeof...(AllArgs) - 1>{});
      }
    }
  }
};
} // namespace pico_detail

// The storage type for all callback outputs of a process
template <typename T>
using avnd_callback_storage_t = typename pico_detail::callback_storage_tuple<T>::type;

// Initialize all callback outputs of a process to write to the storage
template <typename T>
static void avnd_init_callbacks(T& proc, avnd_callback_storage_t<T>& storage)
{
  using outputs_t = typename avnd::outputs_type<T>::type;
  constexpr int N = avnd::output_introspection<T>::size;

  if constexpr(N > 0)
  {
    [&]<std::size_t... Is>(std::index_sequence<Is...>) {
      (
          [&]<std::size_t I>() {
            using field_t =
                typename avnd::output_introspection<T>::template field_type<I>;
            if constexpr(avnd::callback<field_t>)
            {
              auto& field = avnd::output_introspection<T>::template field<I>(proc.outputs);
              auto& store = std::get<I>(storage);
              using store_t = std::remove_cvref_t<decltype(store)>;
              using func_ptr_t = decltype(field.call.function);
              constexpr bool is_timed = avnd::tag_timestamp<field_t>;

              field.call.context = &store;
              field.call.function
                  = &pico_detail::callback_writer<is_timed, store_t, func_ptr_t>::invoke;
            }
          }.template operator()<Is>(),
          ...);
    }(std::make_index_sequence<N>{});
  }
}

// Access an output: for callbacks, return the storage; for regular ports, return the field
template <int N, typename T>
static auto& avnd_get_output(T& proc, avnd_callback_storage_t<T>& cbs)
{
  using field_t = typename avnd::output_introspection<T>::template field_type<N>;
  if constexpr(avnd::callback<field_t>)
    return std::get<N>(cbs);
  else
    return avnd::output_introspection<T>::template field<N>(proc.outputs);
}

// Clean callback storage (reset optionals to nullopt)
template <typename T>
static void avnd_clean_callback_outputs(avnd_callback_storage_t<T>& storage)
{
  std::apply(
      [](auto&... fields) {
        (([]<typename F>(F& f) {
          if constexpr(!std::is_same_v<F, pico_empty_t>)
            f = std::nullopt;
        }(fields)),
         ...);
      },
      storage);
}
