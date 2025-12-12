#pragma once

#include <SDL3/SDL.h>

class Ball {
public:
  Ball() = default;

  void init();
  void update(float dt);
  void render(SDL_Renderer *renderer);
  void reset();

  void bounceX();
  void bounceY();

  void setX(float x);
  void setY(float y);

  const SDL_FRect &getRect() const { return rect_; }

  float getX() const { return rect_.x; }
  float getY() const { return rect_.y; }
  float getVelY() const { return velocityY_; }

private:
  SDL_FRect rect_;
  float velocityX_ = 0.0f;
  float velocityY_ = 0.0f;
};
