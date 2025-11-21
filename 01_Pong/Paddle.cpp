#include "Paddle.hpp"
#include "Constants.hpp"

void Paddle::init(float x, float y) {
  rect_.x = x;
  rect_.y = y;
  rect_.w = PADDLE_WIDTH;
  rect_.h = PADDLE_HEIGHT;

  velocityY_ = 0.0f;
}

void Paddle::update(float dt) {
  rect_.y += velocityY_ * dt;

  if (rect_.y < 0) {
    rect_.y = 0;
  }
  if (rect_.y + rect_.h > SCREEN_HEIGHT) {
    rect_.y = SCREEN_HEIGHT - rect_.h;
  }
}

void Paddle::render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_RenderFillRect(renderer, &rect_);
}

void Paddle::setDirection(int direction) {
  velocityY_ = direction * PADDLE_SPEED;
}
