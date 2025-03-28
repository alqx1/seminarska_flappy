#pragma once

#include <SFML/Audio.hpp>
#include <unordered_map>

enum SoundTag { sJump, sHit, sScore };

class AudioManager {
  public:
    AudioManager();
    void addBuffer(const SoundTag tag, const std::string &filePath);
    void addSound(const SoundTag tag, const std::string &filePath);
    void play(const SoundTag tag);

  private:
    std::unordered_map<SoundTag, sf::SoundBuffer> m_buffers;
    std::unordered_map<SoundTag, sf::Sound> m_sounds;
};
