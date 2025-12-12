#include "Paddle.hpp"
#include "Constants.hpp"

void Paddle::init() {
  rect_.x = (SCREEN_WIDTH - PADDLE_WIDTH) / 2.0f;
  rect_.y = PADDLE_START_Y;
  rect_.w = PADDLE_WIDTH;
  rect_.h = PADDLE_HEIGHT;
  velocityX_ = 0.0f;
}

void Paddle::update(float dt) {
  rect_.x += velocityX_ * dt;

  if (rect_.x < 0) {
    rect_.x = 0;
  }

  if (rect_.x + rect_.w > SCREEN_WIDTH) {
    rect_.x = SCREEN_WIDTH - rect_.w;
  }
}

void Paddle::render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255);
  SDL_RenderFillRect(renderer, &rect_);
}

void Paddle::setDirection(int dir) { velocityX_ = dir * PADDLE_SPEED; }

void Paddle::shrink() { rect_.w = PADDLE_WIDTH / 2.0f; }

void Paddle::resetSize() { rect_.w = PADDLE_WIDTH; }
