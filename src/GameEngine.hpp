#pragma once

#include "managers/AudioManager.hpp"
#include "managers/EntityManager.hpp"
#include "managers/SceneManager.hpp"
#include "managers/TextureManager.hpp"
#include <SFML/Main.hpp>

class GameEngine {
  public:
    GameEngine();
    void run();

  private:
    // Inicializira vse
    void init();

    // Ugasne program
    void turnOff();

    // Poskrbi za dogodke
    void processEvents();

    // Vsi upravljatelji raznih podatkov
    EntityManager m_entityManager;
    SceneManager m_sceneManager;
    TextureManager m_textureManager;
    AudioManager m_audioManager;

    sf::RenderWindow m_window;

    // Določa, kdaj program deluje
    // Če je vrednost false, se program ugasne
    bool m_running = true;
};
