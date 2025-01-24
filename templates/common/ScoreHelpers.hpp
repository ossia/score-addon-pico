#pragma once
#include "ExprtkMapper.hpp"

#include <Fx/LFO_v2.hpp>
#include <Fx/PitchToValue.hpp>
#include <Fx/VelToNote_mono.hpp>

#include <Advanced/Utilities/Automation.hpp>
#include <Advanced/Utilities/ColorAutomation.hpp>
#include <Advanced/Utilities/Smooth.hpp>
#include <avnd/common/export.hpp>
#include <avnd/introspection/input.hpp>
#include <avnd/introspection/messages.hpp>
#include <avnd/introspection/output.hpp>
#include <avnd/introspection/port.hpp>
#include <avnd/wrappers/avnd.hpp>
#include <avnd/wrappers/controls.hpp>
#include <avnd/wrappers/metadatas.hpp>

#include <cmath>

#include <cinttypes>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <avnd/../../examples/Advanced/Utilities/Math.hpp>
struct g_tick_t
{
  int frames{};
  float relative_position{};
} g_tick;

static int get_frames(const g_tick_t& tk)
{
  return tk.frames;
}
static float get_relative_position(const g_tick_t& tk)
{
  return tk.relative_position;
}

static constexpr void avnd_call(auto& proc)
{
  if constexpr(requires { proc(g_tick.frames); })
    proc(g_tick.frames);
  else if constexpr(requires { proc(); })
    proc();
  else
    static_assert(std::is_void_v<decltype(proc)>);
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
      to.value = *from.value;
    }
    else
    {
      to.value = from.value;
    }
  }
  else if constexpr(requires { from.value; })
  {
    if constexpr(requires { *from.value; })

      to = *from.value;
    else
      to = from.value;
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

template<typename T>
static inline constexpr void
midi_adapt(T&& to, T&& from)
{
  to = std::forward<T>(from);
}
static inline  void
midi_adapt(halp::midi_msg& to, libremidi::message& from)
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
    using to_messages_type =  std::remove_cvref_t<decltype(to.midi_messages)>;
    typename to_messages_type::value_type to_msg;
    midi_adapt(to_msg, m);
    to.midi_messages.push_back(std::move(to_msg));
  }
  from.midi_messages.clear(); // FIXME
}

static inline constexpr void
value_adapt(avnd::parameter auto&& to, avnd::parameter auto&& from)
{
  to.value = from.value;
}
template <typename T>
static inline constexpr void
value_adapt(avnd::parameter auto& to, exprtk_arithmetic& from)
{
  to.value = from;
}
template <typename T>
static inline constexpr void
value_adapt(exprtk_arithmetic& to, avnd::parameter auto& from)
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
