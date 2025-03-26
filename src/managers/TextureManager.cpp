#include "TextureManager.hpp"
#include <iostream>

// adds all needed texture needed by game
// not optimized
TextureManager::TextureManager() {
    addTexture(tBird0, "./sprites/yellowbird-upflap.png");
    addTexture(tBird1, "./sprites/yellowbird-midflap.png");
    addTexture(tBird2, "./sprites/yellowbird-downflap.png");
    addTexture(tPipe, "./sprites/pipe-green.png");
    addTexture(tBase, "./sprites/base.png");
    addTexture(tBackground, "./sprites/background-day.png");
    addTexture(tButtons, "./sprites/buttons.png");
    addTexture(tWelcomeText, "./sprites/welcome.png");
    addTexture(tGameOverText, "./sprites/gameover.png");
    addTexture(tGameOver, "./sprites/gameovermenu.png");
    addTexture(tPreMessage, "./sprites/pregamemsg.png");
    addTexture(t0, "./sprites/0.png");
    addTexture(t1, "./sprites/1.png");
    addTexture(t2, "./sprites/2.png");
    addTexture(t3, "./sprites/3.png");
    addTexture(t4, "./sprites/4.png");
    addTexture(t5, "./sprites/5.png");
    addTexture(t6, "./sprites/6.png");
    addTexture(t7, "./sprites/7.png");
    addTexture(t8, "./sprites/8.png");
    addTexture(t9, "./sprites/9.png");
}

// gets texture and exits program if error occurs
sf::Texture &TextureManager::getTexture(const TextureTag tag) {
    auto it = m_textures.find(tag);
    if (it != m_textures.end()) {
        return it->second;
    }
    else {
        std::cerr << "Texture not found" << std::endl;
        exit(1);
    }
}

// adds texture to texture map
void TextureManager::addTexture(
    const TextureTag tag, const std::string &filePath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filePath)) {
        std::cerr << filePath << " didnt load correctly" << std::endl;
        exit(2);
    }
    else {
        m_textures.insert_or_assign(m_textures.begin(), tag, tex);
    }
}
