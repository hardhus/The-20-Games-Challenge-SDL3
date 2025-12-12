#include "SoundManager.hpp"
#include <cmath>
#include <vector>

SoundManager::~SoundManager() { clean(); }

bool SoundManager::init() {
  if (SDL_InitSubSystem(SDL_INIT_AUDIO) == false) {
    SDL_Log("Audio Subsystem baslatilamadi: %s", SDL_GetError());
    return false;
  }

  audioDevice_ =
      SDL_OpenAudioDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, nullptr);

  if (audioDevice_ == 0) {
    SDL_Log("Ses cihazi acilamadi: %s", SDL_GetError());
    return false;
  }

  SDL_AudioSpec spec;
  spec.channels = 1;
  spec.format = SDL_AUDIO_F32;
  spec.freq = 44100;

  audioStream_ = SDL_CreateAudioStream(&spec, &spec);
  if (audioStream_) {
    SDL_BindAudioStream(audioDevice_, audioStream_);
    SDL_ResumeAudioDevice(audioDevice_);
    return true;
  }

  return false;
}

void SoundManager::playTone(float freq, int durationMs) {
  if (!audioStream_)
    return;

  int sampleRate = 44100;
  int sampleCount = (sampleRate * durationMs) / 1000;

  std::vector<float> samples(sampleCount);

  for (int i = 0; i < sampleCount; i++) {
    double time = (double)i / sampleRate;
    float wave = std::sin(2.0f * SDL_PI_F * freq * time);

    // Envelope (Zarf): Sesin son %20'lik kısmında kısılarak bitsin
    float volume = 0.1f;
    if (i > sampleCount * 0.8) {
      float fade =
          1.0f - ((float)(i - sampleCount * 0.8) / (sampleCount * 0.2));
      volume *= fade;
    }

    samples[i] = wave * volume;
  }

  SDL_PutAudioStreamData(audioStream_, samples.data(),
                         samples.size() * sizeof(float));
}

void SoundManager::clean() {
  if (audioStream_) {
    SDL_DestroyAudioStream(audioStream_);
    audioStream_ = nullptr;
  }
  if (audioDevice_) {
    SDL_CloseAudioDevice(audioDevice_);
    audioDevice_ = 0;
  }
}
