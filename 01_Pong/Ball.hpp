#pragma once

#include <SDL3/SDL.h>

class Ball {
public:
  Ball() = default;

  void init();
  void update(float dt);
  void render(SDL_Renderer *renderer);
  void reset();

  void reverseVelocityX();
  void reverseVelocityY();

  void setX(float x);

  const SDL_FRect &getRect() const { return rect_; }

  float getX() const { return x_; }
  float getY() const { return y_; }
  int getSize() const { return size_; }

private:
  SDL_FRect rect_;

  float x_, y_;
  float dx_, dy_;
  int size_;
};
