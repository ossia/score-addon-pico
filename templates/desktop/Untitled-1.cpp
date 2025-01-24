
#include <ossia/editor/scenario/scenario.hpp>
#include <ossia/editor/scenario/time_interval.hpp>
#include <ossia/editor/scenario/time_sync.hpp>
#include <ossia/dataflow/node_process_raw.hpp>
#include <Advanced/Utilities/Automation.hpp>
#include <Advanced/Utilities/ColorAutomation.hpp>
#include <Advanced/Utilities/Mapping.hpp>
#include <Advanced/Utilities/LFO.hpp>
#include <Advanced/Utilities/Smooth.hpp>
#include "ExprtkMapper.hpp"
#include <avnd/introspection/input.hpp>
#include <avnd/introspection/output.hpp>

#include <cstdio>
using namespace ossia;
using namespace ao;
struct Dummy {
  static inline int variable;
 };

struct
{
  int frames{};
  float relative_position{};
} g_tick;

int get_frames(const ossia::token_request& tok) { return (tok.date - tok.prev_date).impl; }

float get_relative_position(const ossia::token_request& tok) { return tok.position(); }
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
  if_possible(to = from)
      else if constexpr(requires { to = *from; }) { if(from) to = *from; }
}


template <static_string address>
static inline constexpr void do_device_read(auto& val)
{
  fprintf(stderr, "Reading: %s : %f\n", address.value, (double) val);
  val = double(rand()) / RAND_MAX;
}

template <static_string address>
static inline constexpr void do_device_write(const auto& val)
{
  fprintf(stderr, "Writing: %s : %f\n", address.value, (double) val);
}

template <static_string address>
static inline constexpr void do_device_write(const std::optional<float>& val)
{
  if(val)
    fprintf(stderr, "Writing: %s : %f\n", address.value, (double) *val);
}

static constexpr void avnd_call(auto& proc)
{
  if constexpr(requires { proc(g_tick.frames); })
    proc(g_tick.frames);
  else if constexpr(requires { proc(); })
    proc();
}

#include "scenario-struct.hpp"
