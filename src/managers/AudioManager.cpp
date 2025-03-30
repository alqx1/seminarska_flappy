#include "AudioManager.hpp"
#include <iostream>

AudioManager::AudioManager() {
    addSound(sHit, "./audio/hit.wav");
    addSound(sJump, "./audio/wing.wav");
    addSound(sScore, "./audio/point.wav");
}

// Doda medpomnilnik v slovar
void AudioManager::addBuffer(const SoundTag tag, const std::string &filePath) {
    sf::SoundBuffer buf;
    if (!buf.loadFromFile(filePath)) {
        std::cerr << filePath << " didnt load correctly" <<  "\n";
        exit(2);
    }
    else {
        m_buffers.insert_or_assign(m_buffers.begin(), tag, buf);
    }
}

// Doda ZVOK v slovar (drugače kot medpomnilnik)
void AudioManager::addSound(const SoundTag tag, const std::string &filePath) {
    addBuffer(tag, filePath);
    auto it = m_buffers.find(tag);
    if (it != m_buffers.end()) {
        sf::Sound s;
        s.setBuffer(it->second);
        m_sounds.insert_or_assign(m_sounds.begin(), tag, s);
    }
    else {
        std::cerr << "Wasnt able to load sound correctly" << "\n";
        exit(3);
    }
}

// Zaigra določen zvok
void AudioManager::play(const SoundTag tag) {
    auto it = m_sounds.find(tag);
    if (it != m_sounds.end()) {
        it->second.play();
    }
    else {
        std::cerr << "Couldnt play sound" << "\n";
    }
}
