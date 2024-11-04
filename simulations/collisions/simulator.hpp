#ifndef _COLLISIONS_SIMULATOR
#define _COLLISIONS_SIMULATOR

#include <chrono>
#include <thread>
#include <sys/types.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <ctime>

namespace simul {

template <class T> struct v2 {
  T x, y;
  template <class U> v2 &operator*=(U const &scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
  }
  template <class U> v2 operator*(U const &scalar) const {
    return {x * scalar, y * scalar};
    return *this;
  }
  template <class U> v2 &operator+=(v2<U> const &v) {
    x += v.x;
    y += v.y;
    return *this;
  }
  template <class U> v2 operator+(v2<U> const &v) const {
    return {x + v.x, y + v.y};
  }
  template <class U> v2 &operator-=(v2<U> const &v) {
    x -= v.x;
    y -= v.y;
    return *this;
  }
  template <class U> v2 operator-(v2<U> const &v) const {
    return {x - v.x, y - v.y};
  }
  v2 operator-() const { return {-x, -y}; }
  v2 operator+() const { return {std::abs(x), std::abs(y)}; }
  v2 &operator--() {
    x = -x;
    y = -y;
    return *this;
  }
  v2 &operator++() {
    x = std::abs(x);
    y = std::abs(y);
    return *this;
  }
  template <class U> v2<U> as() const {
    return {static_cast<U>(x), static_cast<U>(y)};
  }
  v2 &normalize() {
    auto m  = mag();
    x      /= m;
    y      /= m;
    return *this;
  }
  inline T mag() const { return std::sqrt(x * x + y * y); }
  inline T mag_scale() const { return x * x + y * y; }
  v2 unit() const { return {x / mag(), y / mag()}; }
  template <class U> T dot(v2<U> const &v) const { return x * v.x + y * v.y; }

  v2 reflect(v2 const &mirror) const {
    return mirror * (T(2) * this->dot(mirror) / mirror.mag_scale()) - *this;
  }
};

using v2i = v2<int>;
using v2f = v2<float>;

struct ball {
  float radius;
  float velocity;
  v2f position;
  v2f direction;

  inline void adjust(float lapse) { position += direction * velocity * lapse; }
};

enum : uint8_t { OFF = 0x00, RUNNING = 0x01 };

template <std::size_t ball_count> struct Simulator {
  std::array<ball, ball_count> balls;
  uint8_t settings;
  v2i dim, offset;

  Simulator() : balls(), settings(OFF), dim(), offset() {}

  inline void setup_balls() {
    for (auto &ball : balls)
      ball.direction.normalize();
  }

  void simulate() {
    std::clock_t lapse = std::clock();
    setup_balls();
    while (this->settings & RUNNING) {
      float period = (std::clock() - lapse) / 1e6;
      for (auto &ball : balls)
        ball.adjust(period);
      lapse = std::clock();
      collisions();
    }
  }

  inline void set(uint8_t mask) { settings |= mask; }
  inline void unset(uint8_t mask) { settings &= ~mask; }
  inline void clear() { settings = OFF; }

  void collisions() {
    for (auto ball_a = balls.begin(); ball_a != balls.end(); ball_a++)
      for (auto ball_b = ball_a + 1; ball_b != balls.end(); ball_b++)
        collide(*ball_a, *ball_b);
    for (auto &ball : balls)
      reflect_back(ball);
  }

  void reflect_back(ball &ball) {
    float const max_x = dim.x + offset.x - ball.radius,
                max_y = dim.y + offset.y - ball.radius,
                min_x = offset.x + ball.radius, min_y = offset.y + ball.radius;
    if (ball.position.x > max_x) {
      ball.direction.x = -ball.direction.x;
      ball.position.x  = max_x;
    }
    if (ball.position.x < min_x) {
      ball.direction.x = -ball.direction.x;
      ball.position.x  = min_x;
    }
    if (ball.position.y > max_y) {
      ball.direction.y = -ball.direction.y;
      ball.position.y  = max_y;
    }
    if (ball.position.y < min_y) {
      ball.direction.y = -ball.direction.y;
      ball.position.y  = min_y;
    }
  }

  void collide(ball &a, ball &b) {
    auto const min_d   = a.radius + b.radius;
    auto collision_vec = a.position - b.position;
    auto const dist    = collision_vec.mag();
    if (dist > min_d) return;
    if (dist == 0) collision_vec = {1, 1};

    v2f const old_a = a.direction, old_b = b.direction;

    if (old_b.dot(collision_vec) < 0.f)
      b.direction = ((old_a + old_b) * 0.5f).normalize();
    else
      b.direction = (--b.direction).reflect(collision_vec).normalize();

    if (old_a.dot(collision_vec) > 0.f)
      a.direction = ((old_a + old_b) * 0.5f).normalize();
    else
      a.direction = (--a.direction).reflect(-collision_vec).normalize();

    collision_vec.normalize() *= (min_d - dist) / 2;
    a.position                += collision_vec;
    b.position                -= collision_vec;
  }
};
} // namespace simul

#endif // _COLLISIONS_SIMULATOR
