#include <olcPixelGameEngine.hpp>
#include <thread>

struct State {
  olc::vf2d position, direction;
};

template <class T>
olc::v_2d<T> reflect(olc::v_2d<T> const &target, olc::v_2d<T> const &normal) {
  return normal * (T(2) * target.dot(normal) / normal.mag2()) - target;
}

class Directions : public olc::PixelGameEngine {
  bool OnUserCreate() override { return true; }
  bool OnUserDestroy() override { return true; }
  bool OnUserUpdate(float fElapsedTime) override {
    Clear(olc::BLACK);
    auto scale = 90;
    auto w = ScreenWidth() / 3.0f, h = ScreenHeight() / 3.0f;
    olc::vf2d d = olc::vf2d{1, 0}.norm();
    State s1    = {{w, h}, d}, s2{{w + 2 * scale, h + 2 * scale}, -d};
    DrawCircle(s1.position, scale, olc::WHITE);
    DrawCircle(s2.position, scale, olc::WHITE);
    DrawLine(s1.position, s2.position, olc::GREEN);
    DrawLine(s1.position, s1.position + s1.direction * scale, olc::RED);
    DrawLine(s2.position, s2.position + s2.direction * scale, olc::RED);
    olc::vf2d collision_vector = s1.position - s2.position;
#if 0
    olc::vf2d r1 = (collision_vector).reflect(s1.direction).norm(),
              r2 = (-collision_vector).reflect(s2.direction).norm();
#else
    olc::vf2d r1 = reflect(-collision_vector, s1.direction).norm(),
              r2 = reflect(collision_vector, s2.direction).norm();
#endif
    DrawLine(s1.position, s1.position + r1 * scale, olc::YELLOW);
    DrawLine(s2.position, s2.position + r2 * scale, olc::YELLOW);
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return not GetKey(olc::Key::Q).bPressed;
  }
};

int main() {
  Directions app;
  if (app.Construct(1024, 1024, 5, 5))
    app.Start();
}
