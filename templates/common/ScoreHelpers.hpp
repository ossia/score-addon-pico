#pragma once
#include "ExprtkMapper.hpp"

#include <Fx/Arpeggiator.hpp>
#include <Fx/Chord.hpp>
#include <Fx/LFO_v2.hpp>
#include <Fx/PitchToValue.hpp>
#include <Fx/VelToNote_mono.hpp>

#include <Advanced/Utilities/Automation.hpp>
#include <Advanced/Utilities/ColorAutomation.hpp>
#include <Advanced/Utilities/Smooth.hpp>
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

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <avnd/../../examples/Advanced/Utilities/Math.hpp>
static constexpr int sample_rate = 44100;
static constexpr halp::setup g_setup{
    .input_channels = 0, .output_channels = 0, .frames = 128, .rate = sample_rate};

struct g_tick_t
{
  int tick_micros{};
  int frames{};
  float relative_position{};

  int64_t prev_micros{};

  int64_t total_frames{};
  int64_t total_micros{};
  void update_timings()
  {
    auto t = micros();
    if(prev_micros > t)
    {
      auto remaining_prev = std::numeric_limits<decltype(t)>::max() - prev_micros;
      auto total_time = remaining_prev + t;

      frames = sample_rate * total_time / 1e6;
      tick_micros = total_time;
    }
    else
    {
      frames = sample_rate * (t - prev_micros) / 1e6;
      tick_micros = t - prev_micros;
    }
    prev_micros = t;

    total_micros += tick_micros;
    total_frames += frames;
    relative_position += 0.001;
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
    using tick_type = T::tick;
    tick_type tk;
    tk.frames = g_tick.frames;

    if constexpr(requires { tk.position_in_frames; })
      tk.position_in_frames = g_tick.total_frames;
    if constexpr(requires { tk.position_in_nanoseconds; })
      tk.position_in_nanoseconds = g_tick.total_micros * 1e3;
    if constexpr(requires { tk.start_in_flicks; })
      tk.start_in_flicks = g_tick.total_micros * 705.6;
    if constexpr(requires { tk.end_in_flicks; })
      tk.end_in_flicks = (g_tick.total_micros + g_tick.tick_micros) * 705.6;
    if constexpr(requires { tk.relative_position; })
      tk.relative_position = g_tick.relative_position; // FIXME
    if constexpr(requires { tk.parent_duration; })
      tk.parent_duration = 0; // FIXME
    if constexpr(requires { tk.speed; })
      tk.speed = 1.;
    if constexpr(requires { tk.tempo; })
      tk.tempo = 120.;
    if constexpr(requires { tk.start_position_in_quarters; })
    {
      // Tempo = 120 -> 1 quarter note = 1/2 second
      constexpr double quarter_note_duration_in_seconds = 60. / 120.;
      if constexpr(requires { tk.start_position_in_quarters; })
        tk.start_position_in_quarters
            = quarter_note_duration_in_seconds * g_tick.total_micros / 1e6;
      if constexpr(requires { tk.end_position_in_quarters; })
        tk.end_position_in_quarters = quarter_note_duration_in_seconds
                                      * (g_tick.total_micros + g_tick.tick_micros) / 1e6;
      if constexpr(requires { tk.last_signature_change; })
        tk.last_signature_change = 120.;
      if constexpr(requires { tk.bar_at_start; })
        tk.bar_at_start = 0.;
      if constexpr(requires { tk.bar_at_end; })
        tk.bar_at_end = 0.;
    }

    proc(tk);
  }
  //else
  //  static_assert(std::is_void_v<decltype(proc)>);
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

template <typename To, typename From>
static inline constexpr void value_adapt(To&& to, From&& from)
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
      to.value = from.value;
    }
  }
  else if constexpr(requires { from.value; })
  {
    if constexpr(requires { *from.value; }){
      if(from.value)
        to = *from.value;
    }
    else {
      to = from.value;
    }
  }
  else if constexpr(requires { to.value; })
  {
    to.value = from;
  }
  else
  {
    to = from;
  }

  // FIXME
  if constexpr(requires { *from.value; })
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
template <typename T>
static inline constexpr void
value_adapt(avnd::parameter_port auto& to, exprtk_arithmetic& from)
{
  to.value = from;
}
template <typename T>
static inline constexpr void
value_adapt(exprtk_arithmetic& to, avnd::parameter_port auto& from)
{
  to = from.value;
}
template <typename T>
static inline constexpr void value_adapt(auto& to, std::optional<T>& from)
{
  if(from)
    to = *from;
}
template <typename T>
static inline constexpr void value_adapt(auto& to, const std::optional<T>& from)
{
  if(from)
    to = *from;
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
