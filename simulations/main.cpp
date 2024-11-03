#include "../olcpixeng/include/olcPixelGameEngine.h"
#include "bounce.cpp"
#include <chrono>
#include <iostream>
#include <thread>

struct Bounce : public olc::PixelGameEngine {
  std::thread simulator;
  bool simulator_running;
  simul::ball ball;
  simul::rect room;

  Bounce() = delete;
  Bounce(simul::ball const &b, simul::rect const &r)
      : simulator(), simulator_running(), ball(b), room(r) {}

  bool OnUserCreate() override {
    this->simulator_running = true;
    this->simulator =
        std::thread(simul::simulate, std::ref(this->ball),
                    std::cref(this->room), std::ref(simulator_running));
    return true;
  }

  bool OnUserUpdate(float _) override {
    for (int x = 0; x < this->ScreenWidth(); x++)
      for (int y = 0; y < this->ScreenHeight(); y++)
        this->Draw(x, y, olc::BLACK);
    this->DrawRect(room.position.x, room.position.y, room.dim.x, room.dim.y,
                   olc::GREEN);
    this->DrawCircle(ball.position.x, ball.position.y, ball.radius, olc::WHITE);
    /*this->DrawLine(ball.position.x, ball.position.y,*/
    /*               ball.position.x + ball.direction.x * ball.radius * 2,*/
    /*               ball.position.y + ball.direction.y * ball.radius * 2,*/
    /*               olc::RED);*/
    /*this->DrawRect(ball.position.x, ball.position.y, ball.radius, ball.radius,
     * olc::WHITE);*/
    std::cout << "\nFPS = " << this->GetFPS();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));
    return true;
  }

  bool OnUserDestroy() override {
    this->simulator_running = false;
    this->simulator.join();
    return true;
  }
};

int main() {
  const int H = 512, W = 512;
  simul::ball ball{10, 15, {1, 2}, {H / 3.0, W / 3.0}};
  simul::rect room{{6, 6}, {500, 250}};
  Bounce app{ball, room};
  if (app.Construct(H, W, 1, 1))
    app.Start();
}
