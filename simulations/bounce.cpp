#include <cmath>
#include <cstddef>
#include <ctime>
#include <time.h>

namespace simul {
template <class T> struct v2 {
  T x, y;
  v2 unit() const {
    T mag = this->mag();
    return {this->x / mag, this->y / mag};
  }
  T mag() const { return std::sqrt(x * x + y * y); }
  T mag2() const { return std::abs(x) + std::abs(y); }
  template <class U> v2 scale(U scalar) const {
    return {scalar * this->x, scalar * this->y};
  }
  template <class U> v2 operator+(v2<U> const &v) const {
    return {this->x + v.x, this->y + v.y};
  }
  v2 operator-() const { return {-this->x, -this->y}; }
  v2 operator+() const { return {std::abs(this->x), std::abs(this->y)}; }
  template <class U> v2 &operator+=(v2<U> const &v) {
    this->x += v.x;
    this->y += v.y;
    return *this;
  }
  template <class U> v2<U> as() const {
    return {static_cast<U>(this->x), static_cast<U>(this->y)};
  }
};

using v2f = v2<float>;
using v2d = v2<double>;
using v2i = v2<int>;
using v2l = v2<long>;

struct line {
  v2i start, end;
};

struct quad {
  v2f nw, ne, se, sw;
  quad() : nw(), ne(), se(), sw() {};
  quad(v2f const &nw, v2f const &se)
      : nw{nw}, ne{v2f{se.x, nw.y}}, se{se}, sw{v2f{nw.x, se.y}} {};
};

struct ball {
  float radius;
  float velocity;
  v2f direction;
  v2f position;

  void adjust(float t) {
    this->position += this->direction.scale(this->velocity * t);
  }
};

struct rect {
  v2f position;
  v2i dim;
};

void simulate(ball &ball, rect const &room, bool &running) {
  std::clock_t lapse = std::clock();
  float const max_x = room.dim.x + room.position.x - ball.radius,
              max_y = room.dim.y + room.position.y - ball.radius;
  float const min_x = room.position.x + ball.radius,
              min_y = room.position.y + ball.radius;
  ball.direction = ball.direction.unit();
  while (running) {
    ball.adjust((std::clock() - lapse) / 10000.0);
    // Collision detection and resolution
    if (ball.position.x < min_x) {
      ball.position.x = min_x;
      ball.direction.x = -ball.direction.x;
    } else if (ball.position.x > max_x) {
      ball.position.x = max_x;
      ball.direction.x = -ball.direction.x;
    }
    if (ball.position.y < min_y) {
      ball.position.y = min_y;
      ball.direction.y = -ball.direction.y;
    } else if (ball.position.y > max_y) {
      ball.position.y = max_y;
      ball.direction.y = -ball.direction.y;
    }
    lapse = std::clock();
  }
}
} // namespace simul

