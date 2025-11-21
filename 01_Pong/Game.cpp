#include "Game.hpp"
#include "Constants.hpp"
#include "SDL3/SDL_scancode.h"
#include "SoundManager.hpp"
#include <SDL3/SDL.h>

extern "C" {
#include <cstdio>
}

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

  player1_.init(PADDLE_OFFSET_X, (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2);
  player2_.init(SCREEN_WIDTH - PADDLE_OFFSET_X - PADDLE_WIDTH,
                (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2);

  ball_.init();

  lastTick_ = SDL_GetTicks();
  isRunning_ = true;

  SDL_SetWindowTitle(window_, "Yaka - Pong | BASLAMAK ICIN SPACE TUSUNA BASIN");

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

  if (gameState_ == GameState::PLAYING)
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

  if (keyStates[SDL_SCANCODE_SPACE]) {
    if (gameState_ == GameState::MENU) {
      gameState_ = GameState::PLAYING;

      char title[64];
      snprintf(title, sizeof(title), "Yaka - Pong | P1: %d  -  P2: %d", score1_,
               score2_);
      SDL_SetWindowTitle(window_, title);
      SDL_Log("Oyun Basladi!");
    }
  }

  if (keyStates[SDL_SCANCODE_ESCAPE])
    isRunning_ = false;

  if (keyStates[SDL_SCANCODE_R]) {
    resetScores();
  }

  static bool iKeyPressed = false;
  if (keyStates[SDL_SCANCODE_I]) {
    if (!iKeyPressed) {
      isAIActive_ = !isAIActive_;
      SDL_Log("AI Durumu: %s", isAIActive_ ? "AKTIF" : "KAPALI");
      iKeyPressed = true;
    }
  } else {
    iKeyPressed = false;
  }

  int dir1 = 0;
  if (keyStates[SDL_SCANCODE_W])
    dir1 -= 1;
  if (keyStates[SDL_SCANCODE_S])
    dir1 += 1;
  player1_.setDirection(dir1);

  int dir2 = 0;

  if (isAIActive_) {
    // --- İNSANİ YAPAY ZEKA ---

    float targetY; // AI'nın gitmek istediği hedef nokta

    // 1. DURUM ANALİZİ: Top nerede?
    // Eğer top ekranın ortasını (SCREEN_WIDTH / 2) geçtiyse AI heyecanlansın.
    if (ball_.getX() > SCREEN_WIDTH / 2.0f) {
      // Top benim sahamda! Topu takip et.
      targetY = ball_.getY() + (ball_.getSize() / 2.0f);
    } else {
      // Top karşı sahada veya uzakta.
      // Merkeze dön ve bekle (Idle Position).
      targetY = SCREEN_HEIGHT / 2.0f;
    }

    // Raketin şu anki merkezi
    float paddleCenterY = player2_.getRect().y + (PADDLE_HEIGHT / 2.0f);

    // Deadzone (Titreme önleyici bölge)
    float deadzone = 15.0f; // Biraz arttırdık ki çok hassas olmasın

    if (targetY > paddleCenterY + deadzone) {
      dir2 = 1; // Aşağı in
    } else if (targetY < paddleCenterY - deadzone) {
      dir2 = -1; // Yukarı çık
    } else {
      dir2 = 0; // Dur
    }

  } else {

    if (keyStates[SDL_SCANCODE_UP])
      dir2 -= 1;
    if (keyStates[SDL_SCANCODE_DOWN])
      dir2 += 1;
  }
  player2_.setDirection(dir2);
}

void Game::update(float dt) {
  player1_.update(dt);
  player2_.update(dt);
  ball_.update(dt);

  checkCollisions();

  bool scoreChanged = false;

  if (ball_.getX() < 0) {
    score2_++;
    ball_.reset();
    scoreChanged = true;
    SoundManager::Get().playTone(220.0f, 300);
  }

  if (ball_.getX() > SCREEN_WIDTH) {
    score1_++;
    ball_.reset();
    scoreChanged = true;
    SoundManager::Get().playTone(880.0f, 300);
  }

  if (scoreChanged) {
    char title[64];

    snprintf(title, sizeof(title), "Yaka - Pong | P1: %d  -  P2: %d", score1_,
             score2_);
    SDL_SetWindowTitle(window_, title);
  }
}

void Game::render() {
  if (gameState_ == GameState::MENU) {
    SDL_SetRenderDrawColor(renderer_, 20, 20, 40, 255);
  } else {
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
  }
  SDL_RenderClear(renderer_);

  SDL_SetRenderDrawColor(renderer_, 255, 255, 255, 100);
  for (int y = 0; y < SCREEN_WIDTH; y += 20) {
    SDL_FRect line = {(float)SCREEN_WIDTH / 2 - 2, (float)y, 4.0f, 10.0f};
    SDL_RenderFillRect(renderer_, &line);
  }

  player1_.render(renderer_);
  player2_.render(renderer_);
  ball_.render(renderer_);

  if (gameState_ == GameState::MENU) {
    SDL_SetRenderDrawColor(renderer_, 255, 255, 0, 255);
    SDL_FRect startBox = {(SCREEN_WIDTH - 50) / 2.0f,
                          (SCREEN_HEIGHT - 50) / 2.0f, 50.0f, 50.0f};
    SDL_RenderFillRect(renderer_, &startBox);
  }

  SDL_RenderPresent(renderer_);
}

void Game::checkCollisions() {
  const SDL_FRect &ballRect = ball_.getRect();
  const SDL_FRect &p1Rect = player1_.getRect();
  const SDL_FRect &p2Rect = player2_.getRect();

  // --- P1 Carpisma ---
  if (SDL_HasRectIntersectionFloat(&ballRect, &p1Rect)) {
    ball_.reverseVelocityX();

    float newX = p1Rect.x + p1Rect.w + 1.0f;
    ball_.setX(newX);

    SoundManager::Get().playTone(440.0F, 100); // A4 Notası (Bip)

    SDL_Log("Carpisma: Player 1");
  }

  // --- P2 Carpisma ---
  if (SDL_HasRectIntersectionFloat(&ballRect, &p2Rect)) {
    ball_.reverseVelocityX();

    float newX = p2Rect.x - ball_.getSize() - 1.0f;
    ball_.setX(newX);

    SoundManager::Get().playTone(440.0F, 100); // A4 Notası (Bip)

    SDL_Log("Carpisma: Player 2");
  }
}

void Game::resetScores() {
  score1_ = 0;
  score2_ = 0;
  ball_.reset();

  gameState_ = GameState::MENU;

  SDL_SetWindowTitle(window_, "Yaka - Pong | BASLAMAK ICIN SPACE TUSUNA BASIN");

  SDL_Log("Oyun Sifirlandi ve Menuye Donuldu!");
}
