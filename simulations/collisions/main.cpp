#include "../../include/olcPixelGameEngine.h"
#include "simulator.hpp"
#include <chrono>
#include <functional>
#include <iostream>
#include <thread>

struct Settings {
  simul::ball initial_ball_state;
  simul::v2i simulator_dim;
  simul::v2i simulator_off;
};

template <std::size_t N>
struct CollisionSimulator : public olc::PixelGameEngine {
  std::thread worker;
  simul::Simulator<N> simulator;
  simul::ball *target;

  CollisionSimulator(Settings const &s)
      : worker(), simulator(), target(nullptr) {
    simulator.balls.fill(s.initial_ball_state);
    simulator.dim     = s.simulator_dim;
    simulator.offset  = s.simulator_off;

    if constexpr (N == 2 && false) {
      simulator.balls[0].direction = {1, 0.5};
      simulator.balls[1].direction = {-1, 0.5};
      simulator.balls[0].position  = {0, simulator.dim.y / 2.0f +
                                             simulator.offset.y};
      simulator.balls[1].position  = {
          static_cast<float>(simulator.dim.x + simulator.offset.x),
          simulator.dim.y / 2.0f + simulator.offset.y};
    }
  };

  bool OnUserCreate() override {
    simulator.set(simul::RUNNING);
    auto work = std::mem_fn(&simul::Simulator<N>::simulate);
    worker    = std::thread(work, std::ref(simulator));
    return true;
  }

  bool OnUserUpdate(float elapsed_time) override {
    Clear(olc::BLACK);
    for (auto &ball : simulator.balls)
      DrawCircle(ball.position.x, ball.position.y, ball.radius, olc::WHITE);

#ifdef MESH
    for (auto b1 = simulator.balls.begin(); b1 != simulator.balls.end(); b1++)
      for (auto b2 = b1 + 1; b2 != simulator.balls.end(); b2++)
        DrawLine(b1->position.x, b1->position.y, b2->position.x, b2->position.y,
                 olc::DARK_CYAN);
#endif

    if (target)
      FillCircle(target->position.x, target->position.y, target->radius,
                 olc::WHITE);
    DrawRect(simulator.offset.x, simulator.offset.y, simulator.dim.x,
             simulator.dim.y, olc::GREEN);
    if (GetKey(olc::Key::P).bPressed) {
      int index = 0;
      for (auto &ball : simulator.balls) {
        std::cout << "balls[" << index << "] = velocity=" << ball.velocity
                  << ", direction=(" << ball.direction.x << ", "
                  << ball.direction.y << ", mag=" << ball.direction.mag()
                  << "), " << '\n';
        index++;
      }
    }
    if (GetKey(olc::Key::SPACE).bPressed)
      std::this_thread::sleep_for(std::chrono::seconds(5));
    if (GetMouse(olc::Mouse::LEFT).bPressed) {
      auto const &[mx, my] = GetMousePos();
      for (simul::ball &ball : simulator.balls) {
        if ((mx - ball.position.x) * (mx - ball.position.x) +
                (my - ball.position.y) * (my - ball.position.y) <=
            ball.radius * ball.radius) {
          target = &ball;
          break;
        }
      }
    }
    if (GetKey(olc::Key::C).bPressed) target = nullptr;
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
    return not GetKey(olc::Key::Q).bPressed;
  }

  bool OnUserDestroy() override {
    simulator.clear();
    worker.join();
    return true;
  }
};

int main() {
  int const height = 500, width = 500;
  Settings settings{.initial_ball_state =
                        simul::ball{.radius    = 30,
                                    .velocity  = 500,
                                    .position  = {height / 3.0, width / 3.0},
                                    .direction = {1, 2}},
                    .simulator_dim = {.x = width - 12, .y = height - 12},
                    .simulator_off = {.x = 6, .y = 6}};

  CollisionSimulator<9> collisions_simul(settings);
  if (collisions_simul.Construct(width, height, 5, 5)) collisions_simul.Start();
}