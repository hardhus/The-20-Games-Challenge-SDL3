#include "Ball.hpp"
#include "Constants.hpp"
#include "SoundManager.hpp"

void Ball::init() { reset(); }

void Ball::update(float dt) {
  rect_.x += velocityX_ * dt;
  rect_.y += velocityY_ * dt;

  if (rect_.x < 0) {
    rect_.x = 0;
    bounceX();
    SoundManager::Get().playTone(200.0f, 50); // Tok ses
  }

  if (rect_.x + rect_.w > SCREEN_WIDTH) {
    rect_.x = SCREEN_WIDTH - rect_.w;
    bounceX();
    SoundManager::Get().playTone(200.0f, 50);
  }

  if (rect_.y < 0) {
    rect_.y = 0;
    bounceY();
    SoundManager::Get().playTone(200.0f, 50);
  }
}

void Ball::render(SDL_Renderer *renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 50, 50, 255); // Kırmızı Top
  SDL_RenderFillRect(renderer, &rect_);
}

void Ball::reset() {
  rect_.x = (SCREEN_WIDTH - BALL_SIZE) / 2.0f;
  rect_.y = SCREEN_HEIGHT / 2.0f;
  rect_.w = BALL_SIZE;
  rect_.h = BALL_SIZE;

  velocityX_ = BALL_SPEED_BASE * 0.5f;
  velocityY_ = BALL_SPEED_BASE;
}

void Ball::bounceX() { velocityX_ = -velocityX_; }

void Ball::bounceY() { velocityY_ = -velocityY_; }

void Ball::setY(float y) { rect_.y = y; }
void Ball::setX(float x) { rect_.x = x; }
