#pragma once

#include "SDL3/SDL_rect.h"
#include <SDL3/SDL.h>

class Paddle {
public:
  Paddle() = default;

  void init();
  void update(float dt);
  void render(SDL_Renderer *renderer);

  void setDirection(int dir);

  const SDL_FRect &getRect() const { return rect_; }

  void shrink();
  void resetSize();

private:
  SDL_FRect rect_;
  float velocityX_ = 0.0f;
};
