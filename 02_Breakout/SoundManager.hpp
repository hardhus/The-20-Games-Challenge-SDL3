#pragma once

#include <SDL3/SDL.h>

class SoundManager {
public:
  // Global erişim noktası (Meyer's Singleton)
  static SoundManager &Get() {
    static SoundManager instance; // Sadece ilk çağrıldığında oluşur
    return instance;
  }

  // Kopya oluşturmayı engelle (Singleton kuralı)
  SoundManager(const SoundManager &) = delete;
  void operator=(const SoundManager &) = delete;

  bool init();
  void playTone(float freq, int durationMs);
  void clean();

private:
  // Constructor private oldu!
  SoundManager() = default;
  ~SoundManager();

  SDL_AudioDeviceID audioDevice_ = 0;
  SDL_AudioStream *audioStream_ = nullptr;
};
