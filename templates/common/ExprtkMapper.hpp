#pragma once
#include <span>
#include <optional>
#include <vector>
#include <compare>
#include <cmath>
#include <ranges>

struct exprtk_arithmetic
{
  constexpr exprtk_arithmetic() = default;
  constexpr exprtk_arithmetic(const exprtk_arithmetic&) = default;
  constexpr exprtk_arithmetic(exprtk_arithmetic&&) = default;
  constexpr exprtk_arithmetic& operator=(const exprtk_arithmetic&) = default;
  constexpr exprtk_arithmetic& operator=(exprtk_arithmetic&&) = default;
  constexpr exprtk_arithmetic(float val): v{val} { }
  constexpr operator float&() noexcept { return v; }
  constexpr operator const float&() const noexcept { return v; }
  float v{};

  bool operator==(const exprtk_arithmetic& rhs) const noexcept = default;
  auto operator<=>(const exprtk_arithmetic& rhs) const noexcept = default;
  friend exprtk_arithmetic operator+(exprtk_arithmetic lhs, exprtk_arithmetic rhs) noexcept { return lhs.v + rhs.v; }
  friend exprtk_arithmetic operator-(exprtk_arithmetic lhs, exprtk_arithmetic rhs) noexcept { return lhs.v - rhs.v; }
  friend exprtk_arithmetic operator*(exprtk_arithmetic lhs, exprtk_arithmetic rhs) noexcept { return lhs.v * rhs.v; }
  friend exprtk_arithmetic operator/(exprtk_arithmetic lhs, exprtk_arithmetic rhs) noexcept { return lhs.v / rhs.v; }
  friend exprtk_arithmetic operator%(exprtk_arithmetic lhs, exprtk_arithmetic rhs) noexcept { return std::fmod(lhs.v, rhs.v); }

  friend exprtk_arithmetic operator+(exprtk_arithmetic lhs, float rhs) noexcept { return lhs.v + rhs; }
  friend exprtk_arithmetic operator-(exprtk_arithmetic lhs, float rhs) noexcept { return lhs.v - rhs; }
  friend exprtk_arithmetic operator*(exprtk_arithmetic lhs, float rhs) noexcept { return lhs.v * rhs; }
  friend exprtk_arithmetic operator/(exprtk_arithmetic lhs, float rhs) noexcept { return lhs.v / rhs; }
  friend exprtk_arithmetic operator%(exprtk_arithmetic lhs, float rhs) noexcept { return std::fmod(lhs.v, rhs); }
  friend exprtk_arithmetic operator+(float lhs, exprtk_arithmetic rhs) noexcept { return lhs + rhs.v; }
  friend exprtk_arithmetic operator-(float lhs, exprtk_arithmetic rhs) noexcept { return lhs - rhs.v; }
  friend exprtk_arithmetic operator*(float lhs, exprtk_arithmetic rhs) noexcept { return lhs * rhs.v; }
  friend exprtk_arithmetic operator/(float lhs, exprtk_arithmetic rhs) noexcept { return lhs / rhs.v; }
  friend exprtk_arithmetic operator%(float lhs, exprtk_arithmetic rhs) noexcept { return std::fmod(lhs, rhs.v); }

  friend exprtk_arithmetic operator+(exprtk_arithmetic lhs, double rhs) noexcept { return lhs.v + rhs; }
  friend exprtk_arithmetic operator-(exprtk_arithmetic lhs, double rhs) noexcept { return lhs.v - rhs; }
  friend exprtk_arithmetic operator*(exprtk_arithmetic lhs, double rhs) noexcept { return lhs.v * rhs; }
  friend exprtk_arithmetic operator/(exprtk_arithmetic lhs, double rhs) noexcept { return lhs.v / rhs; }
  friend exprtk_arithmetic operator%(exprtk_arithmetic lhs, double rhs) noexcept { return std::fmod(lhs.v, rhs); }
  friend exprtk_arithmetic operator+(double lhs, exprtk_arithmetic rhs) noexcept { return lhs + rhs.v; }
  friend exprtk_arithmetic operator-(double lhs, exprtk_arithmetic rhs) noexcept { return lhs - rhs.v; }
  friend exprtk_arithmetic operator*(double lhs, exprtk_arithmetic rhs) noexcept { return lhs * rhs.v; }
  friend exprtk_arithmetic operator/(double lhs, exprtk_arithmetic rhs) noexcept { return lhs / rhs.v; }
  friend exprtk_arithmetic operator%(double lhs, exprtk_arithmetic rhs) noexcept { return std::fmod(lhs, rhs.v); }

  friend exprtk_arithmetic operator+(exprtk_arithmetic lhs, int rhs) noexcept { return lhs.v + rhs; }
  friend exprtk_arithmetic operator-(exprtk_arithmetic lhs, int rhs) noexcept { return lhs.v - rhs; }
  friend exprtk_arithmetic operator*(exprtk_arithmetic lhs, int rhs) noexcept { return lhs.v * rhs; }
  friend exprtk_arithmetic operator/(exprtk_arithmetic lhs, int rhs) noexcept { return lhs.v / rhs; }
  friend exprtk_arithmetic operator%(exprtk_arithmetic lhs, int rhs) noexcept { return std::fmod(lhs.v, rhs); }
  friend exprtk_arithmetic operator+(int lhs, exprtk_arithmetic rhs) noexcept { return lhs + rhs.v; }
  friend exprtk_arithmetic operator-(int lhs, exprtk_arithmetic rhs) noexcept { return lhs - rhs.v; }
  friend exprtk_arithmetic operator*(int lhs, exprtk_arithmetic rhs) noexcept { return lhs * rhs.v; }
  friend exprtk_arithmetic operator/(int lhs, exprtk_arithmetic rhs) noexcept { return lhs / rhs.v; }
  friend exprtk_arithmetic operator%(int lhs, exprtk_arithmetic rhs) noexcept { return std::fmod(lhs, rhs.v); }

  friend exprtk_arithmetic operator+(exprtk_arithmetic lhs, int64_t rhs) noexcept { return lhs.v + rhs; }
  friend exprtk_arithmetic operator-(exprtk_arithmetic lhs, int64_t rhs) noexcept { return lhs.v - rhs; }
  friend exprtk_arithmetic operator*(exprtk_arithmetic lhs, int64_t rhs) noexcept { return lhs.v * rhs; }
  friend exprtk_arithmetic operator/(exprtk_arithmetic lhs, int64_t rhs) noexcept { return lhs.v / rhs; }
  friend exprtk_arithmetic operator%(exprtk_arithmetic lhs, int64_t rhs) noexcept { return std::fmod(lhs.v, rhs); }
  friend exprtk_arithmetic operator+(int64_t lhs, exprtk_arithmetic rhs) noexcept { return lhs + rhs.v; }
  friend exprtk_arithmetic operator-(int64_t lhs, exprtk_arithmetic rhs) noexcept { return lhs - rhs.v; }
  friend exprtk_arithmetic operator*(int64_t lhs, exprtk_arithmetic rhs) noexcept { return lhs * rhs.v; }
  friend exprtk_arithmetic operator/(int64_t lhs, exprtk_arithmetic rhs) noexcept { return lhs / rhs.v; }
  friend exprtk_arithmetic operator%(int64_t lhs, exprtk_arithmetic rhs) noexcept { return std::fmod(lhs, rhs.v); }

};

struct ExprtkMapper_iS_oS
{
  struct I {
    struct P0 { std::optional<exprtk_arithmetic> value; } in;
    struct P1 { exprtk_arithmetic value; } a;
    struct P2 { exprtk_arithmetic value; } b;
    struct P3 { exprtk_arithmetic value; } c;
    struct P4 { exprtk_arithmetic value; } m1;
    struct P5 { exprtk_arithmetic value; } m2;
    struct P6 { exprtk_arithmetic value; } m3;
  } inputs;

  struct {

    struct {  std::optional<exprtk_arithmetic> value; } out;
  } outputs;
};

// FIXME optional
struct ExprtkMapper_iS_oV
{
  struct I {
    struct P0 {  exprtk_arithmetic value; } in;
    struct P1 {  exprtk_arithmetic value; } a;
    struct P2 {  exprtk_arithmetic value; } b;
    struct P3 {  exprtk_arithmetic value; } c;
    struct P4 {  exprtk_arithmetic value; } m1;
    struct P5 {  exprtk_arithmetic value; } m2;
    struct P6 {  exprtk_arithmetic value; } m3;
  } inputs;

  struct {

    struct { std::vector< exprtk_arithmetic> value; } out;
  } outputs;
};

struct ExprtkMapper_iV_oS
{
  struct I {
    struct P0 { std::span< exprtk_arithmetic>  value; } in;
    struct P1 {  exprtk_arithmetic value; } a;
    struct P2 {  exprtk_arithmetic value; } b;
    struct P3 {  exprtk_arithmetic value; } c;
    struct P4 {  exprtk_arithmetic value; } m1;
    struct P5 {  exprtk_arithmetic value; } m2;
    struct P6 {  exprtk_arithmetic value; } m3;
  } inputs;

  struct {

    struct {  exprtk_arithmetic value; } out;
  } outputs;
};
struct ExprtkMapper_iV_oV
{
  struct I {
    struct P0 { std::span< exprtk_arithmetic> value; } in;
    struct P1 {  exprtk_arithmetic value; } a;
    struct P2 {  exprtk_arithmetic value; } b;
    struct P3 {  exprtk_arithmetic value; } c;
    struct P4 {  exprtk_arithmetic value; } m1;
    struct P5 {  exprtk_arithmetic value; } m2;
    struct P6 {  exprtk_arithmetic value; } m3;
  } inputs;

  struct {

    struct { std::vector< exprtk_arithmetic> value; } out;
  } outputs;
};

struct ExprtkArrayGen
{
  struct I {
    struct P0 { int value; } sz;
  } inputs;

  struct {
    struct { std::vector<exprtk_arithmetic> value; } out;
  } outputs;
};

struct ExprtkArrayMap
{
  struct I {
    struct P0 { std::optional<std::vector<exprtk_arithmetic>> value; } in;
  } inputs;

  struct {
    struct { std::optional<std::vector<exprtk_arithmetic>> value; } out;
  } outputs;
};

// exprtk built-in functions not available in C++ standard library
static inline float exprtk_sgn(float x) noexcept
{
  return (x > 0.f) ? 1.f : ((x < 0.f) ? -1.f : 0.f);
}
static inline float exprtk_frac(float x) noexcept
{
  return x - std::floor(x);
}
static inline float exprtk_clamp(float lo, float v, float hi) noexcept
{
  return (v < lo) ? lo : ((v > hi) ? hi : v);
}
static inline float exprtk_iclamp(float lo, float v, float hi) noexcept
{
  if(v >= lo && v <= hi)
  {
    float mid = (lo + hi) * 0.5f;
    return (v < mid) ? lo : hi;
  }
  return v;
}
static inline float exprtk_inrange(float lo, float v, float hi) noexcept
{
  return (lo <= v && v <= hi) ? 1.f : 0.f;
}
static inline float exprtk_root(float x, float n) noexcept
{
  return std::pow(x, 1.f / n);
}
static inline float exprtk_roundn(float x, float n) noexcept
{
  float p = std::pow(10.f, n);
  return std::round(x * p) / p;
}
static inline float exprtk_logn(float x, float n) noexcept
{
  return std::log(x) / std::log(n);
}
static inline float exprtk_ncdf(float x) noexcept
{
  return 0.5f * (1.f + std::erf(x / std::sqrt(2.f)));
}
static inline float exprtk_sinc(float x) noexcept
{
  return (x == 0.f) ? 1.f : (std::sin(x) / x);
}
static inline float exprtk_cot(float x) noexcept
{
  return std::cos(x) / std::sin(x);
}
static inline float exprtk_csc(float x) noexcept
{
  return 1.f / std::sin(x);
}
static inline float exprtk_sec(float x) noexcept
{
  return 1.f / std::cos(x);
}
static inline float exprtk_deg2rad(float x) noexcept
{
  return x * float(M_PI) / 180.f;
}
static inline float exprtk_rad2deg(float x) noexcept
{
  return x * 180.f / float(M_PI);
}
static inline float exprtk_deg2grad(float x) noexcept
{
  return x * 10.f / 9.f;
}
static inline float exprtk_grad2deg(float x) noexcept
{
  return x * 9.f / 10.f;
}
static inline float exprtk_equal(float a, float b) noexcept
{
  return (a == b) ? 1.f : 0.f;
}
static inline float exprtk_not_equal(float a, float b) noexcept
{
  return (a != b) ? 1.f : 0.f;
}
