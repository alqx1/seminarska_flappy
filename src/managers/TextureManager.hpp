#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

enum TextureTag {
    tBird0,
    tBird1,
    tBird2,
    tPipe,
    tBase,
    tBackground,
    tButtons,
    tWelcomeText,
    tGameOverText,
    tGameOver,
    tPreMessage,
    t0,
    t1,
    t2,
    t3,
    t4,
    t5,
    t6,
    t7,
    t8,
    t9
};

class TextureManager {
  public:
    TextureManager();
    sf::Texture &getTexture(const TextureTag tag);
    void addTexture(const TextureTag tag, const std::string &filePath);

  private:
    std::unordered_map<TextureTag, sf::Texture> m_textures;
};
