#include "Game.hpp"
#include "Constants.hpp"
#include "SoundManager.hpp"
#include <SDL3/SDL.h>

Game::~Game() {
  SDL_Log("Oyun kapatiliyor, temizlik yapiliyor...");

  if (renderer_)
    SDL_DestroyRenderer(renderer_);
  if (window_)
    SDL_DestroyWindow(window_);

  SoundManager::Get().clean();

  SDL_Quit();
}

bool Game::init(const char *title, int width, int height) {
  screenWidth_ = width;
  screenHeight_ = height;

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == false) {
    SDL_Log("SDL Init hatası: %s", SDL_GetError());
    return false;
  }

  if (SDL_CreateWindowAndRenderer(title, width, height, 0, &window_,
                                  &renderer_) == false) {
    SDL_Log("Pencere oluşturma hatası: %s", SDL_GetError());
    return false;
  }

  SDL_SetRenderLogicalPresentation(renderer_, SCREEN_WIDTH, SCREEN_HEIGHT,
                                   SDL_LOGICAL_PRESENTATION_LETTERBOX);

  SoundManager::Get().init();

  paddle_.init();
  ball_.init();

  lastTick_ = SDL_GetTicks();
  isRunning_ = true;

  SDL_SetWindowTitle(window_, "Yaka - Breakout");

  SDL_Log("Oyun basariyla baslatildi!");
  return true;
}

void Game::run() {
  while (isRunning_) {
    loop();
  }
}

void Game::loop() {
  Uint64 currentTick = SDL_GetTicks();
  float dt = (currentTick - lastTick_) / 1000.0f;
  lastTick_ = currentTick;

  if (dt > 0.05f)
    dt = 0.05f;

  processEvents();
  update(dt);
  render();
}

void Game::processEvents() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_EVENT_QUIT) {
      isRunning_ = false;
    }
  }

  // --- KLAVYE KONTROLÜ ---
  // SDL3'te klavye durumunu böyle alıyoruz
  const bool *keyStates = SDL_GetKeyboardState(nullptr);

  if (keyStates[SDL_SCANCODE_ESCAPE])
    isRunning_ = false;

  int dir = 0;

  if (keyStates[SDL_SCANCODE_A] || keyStates[SDL_SCANCODE_LEFT])
    dir = -1;

  if (keyStates[SDL_SCANCODE_D] || keyStates[SDL_SCANCODE_RIGHT])
    dir = 1;

  paddle_.setDirection(dir);
}

void Game::update(float dt) {
  paddle_.update(dt);
  ball_.update(dt);

  checkCollisions();

  if (ball_.getY() > SCREEN_HEIGHT) {
    ball_.reset();
    SoundManager::Get().playTone(100.0f, 500); // Başarısızlık sesi (Kalın)
    SDL_Log("Can Kaybi!");
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  SDL_RenderClear(renderer_);

  paddle_.render(renderer_);
  ball_.render(renderer_);

  SDL_RenderPresent(renderer_);
}

void Game::checkCollisions() {
  if (SDL_HasRectIntersectionFloat(&ball_.getRect(), &paddle_.getRect())) {
    if (ball_.getVelY() > 0) {
      ball_.bounceY();
      ball_.setY(paddle_.getRect().y - BALL_SIZE - 1.0f);

      SoundManager::Get().playTone(440.0f, 100); // Bip
    }
  }
}
