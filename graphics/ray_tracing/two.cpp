#include "utils.hpp"
#include <cmath>
#include <olcPixelGameEngine.hpp>
#include <utility>

using utils::p3;

struct RayTracer: olc::PixelGameEngine {
  utils::Scene scene;
  float step = 0.25f;

  RayTracer(utils::Scene scene): olc::PixelGameEngine(), scene(std::move(scene)) {
    sAppName = "Ray Tracing Second Attempt";
  }

  bool OnUserCreate() override { return true; }
  bool OnUserDestroy() override { return true; }

  bool OnUserUpdate(float fElapsedTime) override {
    const int Cw = ScreenWidth(), Ch = ScreenHeight();

    auto canvas2viewport = [&] (int Cx, int Cy) -> p3 {
      return {
        (scene.view_port.x * Cx) / Cw,
        (scene.view_port.y * Cy) / Ch,
        scene.view_port.z
      };
    };

    auto solve_eq = [&] (p3 const &V, utils::Sphere const &sphere) {
      auto CO = scene.camera - sphere.center;
      auto D = scene.camera - V;
      auto a = D.sqr();
      auto b = 2 * CO.dot(D);
      auto c = CO.sqr() - sphere.radius * sphere.radius;
      auto discr = b * b - 4 * a * c;
      if ( discr < 0 ) return INFINITY;
      auto t1 = (b - std::sqrt(discr)) / (a * 2);
      auto t2 = (b + std::sqrt(discr)) / (a * 2);
      return t1 < t2 ? t1 : t2;
    };

    auto trace_ray = [&] (int Cx, int Cy) -> olc::Pixel {
      auto V = canvas2viewport(Cx, Cy);
      float closest_t = INFINITY;
      olc::Pixel closest_color = scene.background;
      for (auto const &sphere : scene.spheres) {
        if (float t = solve_eq(V, sphere); t > 1 and t < closest_t) {
          closest_t = t;
          closest_color = sphere.color;
        }
      }
      return closest_color;
    };

    for (int y = 0, Cy = Ch / 2; y < Ch; y++, Cy--) {
      for (int x = 0, Cx = -Cw / 2; x < Cw; x++, Cx++) {
        Draw(x, y, trace_ray(Cx, Cy));
      }
    }

    if (GetKey(olc::Key::LEFT).bPressed) step *= 2;
    if (GetKey(olc::Key::RIGHT).bPressed) step /= 2;
    if (GetKey(olc::Key::UP).bPressed) scene.camera.z += step;
    if (GetKey(olc::Key::DOWN).bPressed) scene.camera.z -= step;
    if (GetKey(olc::Key::SPACE).bPressed) std::cout << "Step: " << step << " CamZ: " << scene.camera.z << '\n';
    std::cerr << GetFPS() << '\n';
    return true;
  }
};

int main() {
  RayTracer tracer(utils::Scene {
    .background=olc::GREY,
    .view_port={1, 1, 1},
    .camera={0, 0, 0},
    .direction={0, 0, 1},
    .spheres={
      {.center = {0, -1, 3}, .radius = 1, .color = olc::RED},
      {.center = {2, 0, 4}, .radius = 1, .color = olc::BLUE},
      {.center = {-2, 0, 4}, .radius = 1, .color = olc::GREEN},
      {.center={0, 0, 10}, .radius=1, .color=olc::DARK_CYAN},
      {.center={0, 0, 20}, .radius=1, .color=olc::DARK_YELLOW},
      {.center={0, 5, 15}, .radius=1, .color=olc::DARK_RED},
      {.center={-3, 0, 25}, .radius=2, .color=olc::YELLOW},
    }
  });
  if(tracer.Construct(200, 200, 1, 1))
    tracer.Start();
}

