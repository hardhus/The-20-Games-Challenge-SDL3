#include "Ball.hpp"
#include "Constants.hpp"
#include "SoundManager.hpp"

void Ball::init() {
  size_ = BALL_SIZE;
  reset();
}

void Ball::update(float dt) {
  x_ += dx_ * dt;
  y_ += dy_ * dt;

  rect_.x = x_;
  rect_.y = y_;

  if (y_ < 0) {
    y_ = 0;
    dy_ = -dy_;

    SoundManager::Get().playTone(800.0f, 50);
  }

  if (y_ + size_ > SCREEN_HEIGHT) {
    y_ = SCREEN_HEIGHT - size_;
    dy_ = -dy_;
    SoundManager::Get().playTone(800.0f, 50);
  }
}

void Ball::render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_FRect rect = {x_, y_, static_cast<float>(size_),
                    static_cast<float>(size_)};

  SDL_RenderFillRect(renderer, &rect);
}

void Ball::reset() {
  x_ = (SCREEN_WIDTH - size_) / 2.0f;
  y_ = (SCREEN_HEIGHT - size_) / 2.0f;

  rect_ = {x_, y_, static_cast<float>(size_), static_cast<float>(size_)};

  dx_ = BALL_SPEED_X;
  dy_ = BALL_SPEED_Y;
}

void Ball::reverseVelocityX() { dx_ *= -1; }

void Ball::reverseVelocityY() { dy_ *= -1; }

void Ball::setX(float x) {
  x_ = x;
  rect_.x = x;
}
