#pragma once

#include <SDL3/SDL_rect.h>
#include <SDL3/SDL_render.h>

class Paddle {
public:
  Paddle() = default;

  void init(float x, float y);

  void update(float dt);
  void render(SDL_Renderer *renderer);

  void setDirection(int direction);

  const SDL_FRect &getRect() const { return rect_; }

private:
  SDL_FRect rect_;
  float velocityY_ = 0.0f;
};
