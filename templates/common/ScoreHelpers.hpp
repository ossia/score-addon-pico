#pragma once
#include "ExprtkMapper.hpp"

#include <Fx/LFO_v2.hpp>
#include <Fx/PitchToValue.hpp>
#include <Fx/VelToNote.hpp>

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

static inline constexpr void value_adapt(auto&& to, auto&& from)
{
  to = from;
}
static inline constexpr void
value_adapt(avnd::midi_port auto&& to, avnd::midi_port auto&& from)
{
  to = from;
}
static inline constexpr void
value_adapt(avnd::parameter auto&& to, avnd::parameter auto&& from)
{
  to.value = from.value;
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
