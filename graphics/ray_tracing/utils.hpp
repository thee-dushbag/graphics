#pragma once

#include <cmath>
#include <olcPixelGameEngine.hpp>
#include <vector>

namespace utils {
template <class T> struct p3t {
  p3t() : x(), y(), z() {}
  p3t(T a) : x(a), y(a), z(a) {}
  p3t(T x, T y, T z) : x(x), y(y), z(z) {}

  inline T sum() const { return x + y + z; }
  inline T sqr() const { return dot(*this); }
  inline T mag() const { return std::sqrt(sqr()); }
  inline T dot(p3t const &p) const { return (*this * p).sum(); }
  inline p3t unit() const { return *this / mag(); }
  inline p3t operator*(T t) const { return {t * x, t * y, t * z}; }
  inline p3t operator-(T t) const { return {x - t, y - t, z - t}; }
  inline p3t operator+(T t) const { return {x + t, y + t, z + t}; }
  inline p3t operator/(T t) const { return {x / t, y / t, z / t}; }

  inline p3t operator-(p3t const &p) const {
    return {x - p.x, y - p.y, z - p.z};
  }
  inline p3t operator+(p3t const &p) const {
    return {x + p.x, y + p.y, z + p.z};
  }
  inline p3t operator*(p3t const &p) const {
    return {x * p.x, y * p.y, z * p.z};
  }
  inline p3t operator/(p3t const &p) const {
    return {x / p.x, y / p.y, z / p.z};
  }
  T x, y, z;
};

using p3i = p3t<int>;
using p3f = p3t<float>;
using p3  = p3f;

struct Sphere {
  p3 center;
  int radius;
  olc::Pixel color;
};

struct Scene {
  olc::Pixel background;
  p3 view_port, camera, direction;
  std::vector<Sphere> spheres;
  float t_max, t_min;
};
}
