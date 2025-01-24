#pragma once
#include <span>
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
    struct P0 { exprtk_arithmetic value; } in;
    struct P1 { exprtk_arithmetic value; } a;
    struct P2 { exprtk_arithmetic value; } b;
    struct P3 { exprtk_arithmetic value; } c;
    struct P4 { exprtk_arithmetic value; } m1;
    struct P5 { exprtk_arithmetic value; } m2;
    struct P6 { exprtk_arithmetic value; } m3;
  } inputs;

  struct {

    struct {  exprtk_arithmetic value; } out;
  } outputs;
};

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
