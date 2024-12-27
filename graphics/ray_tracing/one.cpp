#include <cmath>
#include <float.h>
#include <olcPixelGameEngine.hpp>
#include <vector>
#include "utils.hpp"

using utils::p3;

struct Sphere {
  p3 center;
  int radius;
  olc::Pixel color;
};

struct Scene {
  olc::Pixel background;
  p3 view_port, camera;
  std::vector<Sphere> spheres;
  float t_max, t_min;
};

struct Application : olc::PixelGameEngine {
  Scene scene;
  Application() : olc::PixelGameEngine(), scene() {}
  Application(Scene scene) : olc::PixelGameEngine(), scene{std::move(scene)} {}

  bool OnUserCreate() override { return true; }

  bool OnUserUpdate(float fElapsedTime) override {
    const int W = ScreenWidth(), H = ScreenHeight();
    const auto canvas2viewport = [&](int x, int y) -> p3 {
      return {x * scene.view_port.x / (float)W,
              -y * scene.view_port.y / (float)H, scene.view_port.z};
    };
    const auto intersect_ray_sphere = [&](p3 const &camera, p3 const &direction,
                                          Sphere const &sphere) -> olc::vf2d {
      const auto rr = sphere.radius * sphere.radius;
      const auto co = camera - sphere.center;
      auto a  = direction.dot(direction);
      auto b  = 2 * co.dot(direction);
      auto c  = co.dot(co) - rr;

      auto discr    = b * b - 4 * a * c;
      if (discr < 0)
        return {INFINITY, INFINITY};
      discr = std::sqrt(discr) / (2 * a);
      return {-b + discr, b + discr};
    };
    const auto trace_ray = [&](p3 const &camera, p3 const &direction) {
      float closest_t          = INFINITY;
      olc::Pixel closest_color = scene.background;
      for (auto const &sphere : scene.spheres) {
        auto [t1, t2] = intersect_ray_sphere(camera, direction, sphere);
        if (t1 < closest_t) {
          closest_t     = t1;
          closest_color = sphere.color;
        }
        if (t2 < closest_t) {
          closest_t     = t2;
          closest_color = sphere.color;
        }
      }
      return closest_color;
    };
    for (int x = -W / 2, px = 0; x <= W / 2; x++, px++) {
      for (int y = -H / 2, py = 0; y <= H / 2; y++, py++) {
        auto vp = canvas2viewport(x, y);
        Draw(px, py, trace_ray(scene.camera, vp - scene.camera));
      }
    }
    return not GetKey(olc::Key::Q).bPressed;
  }

  bool OnUserDestroy() override { return true; }
};

int main() {
  const int WIDTH = 500, HEIGHT = WIDTH, PWIDTH = 5, PHEIGHT = PWIDTH;
  // clang-format off
  Scene scene{
      .background = olc::WHITE,
      .view_port  = {1, 1, 1},
      .camera     = {0, 0, 0},
      .spheres    = {
          {.center = {0, -1, 3}, .radius = 1, .color = olc::RED},
          {.center = {2, 0, 4}, .radius = 1, .color = olc::BLUE},
          {.center = {-2, 0, 4}, .radius = 1, .color = olc::GREEN},
      }
  };
  // clang-format on

  Application app(std::move(scene));
  if (app.Construct(WIDTH, HEIGHT, PWIDTH, PHEIGHT))
    app.Start();
}

