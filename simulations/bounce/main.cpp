#include "bounce.cpp"
#include <iostream>
#include <olcPixelGameEngine.hpp>
#include <thread>

struct Bounce : public olc::PixelGameEngine {
  std::thread simulator;
  bool simulator_running;
  simul::ball ball;
  simul::rect room;

  Bounce() = delete;
  Bounce(simul::ball const &b, simul::rect const &r)
      : simulator(), simulator_running(), ball(b), room(r) {
  }

  bool OnUserCreate() override {
    this->simulator_running = true;
    this->simulator =
        std::thread(simul::simulate, std::ref(this->ball),
                    std::cref(this->room), std::ref(simulator_running));
    return true;
  }

  bool OnUserUpdate(float _) override {
    this->Clear(olc::BLACK);
    this->DrawRect(room.position.x, room.position.y, room.dim.x, room.dim.y,
                   olc::GREEN);
    /*this->FillRect(ball.position.x - ball.radius, ball.position.y - ball.radius,*/
                   /*ball.radius * 2, ball.radius * 2, olc::WHITE);*/
    this->FillCircle(ball.position.x, ball.position.y, ball.radius, olc::WHITE);
    /*this->DrawLine(ball.position.x, ball.position.y,*/
    /*               ball.position.x + ball.direction.x * ball.radius * 2,*/
    /*               ball.position.y + ball.direction.y * ball.radius * 2,*/
    /*               olc::RED);*/
    /*this->DrawRect(ball.position.x, ball.position.y, ball.radius, ball.radius,
     * olc::WHITE);*/
    /*std::cout << "\rFPS = " << this->GetFPS();*/
    /*std::this_thread::sleep_for(std::chrono::milliseconds(1000 / 30));*/
    if (this->GetKey(olc::Key::Q).bPressed)
      return false;
    if(this->GetKey(olc::Key::F).bPressed)
      std::cout << "FPS = " << this->GetFPS() << '\n';
    return true;
  }

  bool OnUserDestroy() override {
    this->simulator_running = false;
    this->simulator.join();
    return true;
  }
};

int main() {
  const int H = 1024, W = 512;
  simul::ball ball{50, 5, {6, 2}, {H / 3.0, W / 3.0}};
  simul::rect room{{12, 12}, {H - 24, W - 24}};
  Bounce app{ball, room};
  if (app.Construct(H, W, 15, 15))
    app.Start();
}
