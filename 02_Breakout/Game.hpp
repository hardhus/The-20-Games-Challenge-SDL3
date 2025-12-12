#pragma once

#include "Ball.hpp"
#include "Paddle.hpp"
#include <SDL3/SDL.h>

class Game {
public:
  Game() = default;
  ~Game();

  bool init(const char *title, int width, int height);
  void run();
  void loop();

private:
  void processEvents();
  void update(float dt);
  void render();

  void checkCollisions();

  SDL_Window *window_ = nullptr;
  SDL_Renderer *renderer_ = nullptr;

  bool isRunning_ = false;

  int screenWidth_ = 0;
  int screenHeight_ = 0;

  Uint64 lastTick_ = 0;

  Paddle paddle_;
  Ball ball_;
};
