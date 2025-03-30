#pragma once

#include "../managers/AudioManager.hpp"
#include "../managers/EntityManager.hpp"
#include "../managers/InputManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"
#include <SFML/System/Time.hpp>

class SceneManager;

// Bazni razred, ki ga uporabljajo vse ostale scene
class Scene {
  public:
    // Inicializira, če scena še ne obstaja
    virtual void init() = 0;
    // Poganja delovanje
    virtual void run(const sf::Time dt) = 0;

    // Ko se scene uveljavi kot glavno spet
    virtual void onActivate() {};
    virtual void onDeactivate() {};

  private:
    // Sistemi, ki opravljajo vsi svojo stvar
    virtual void sSpawner(const sf::Time dt) {};
    virtual void sInput() {};
    virtual void sPhysics() {};
    virtual void sMovement(const sf::Time dt) {};
    virtual void sCollision() {};
    virtual void sAnimation() {};
    virtual void sRender() {};

  protected:
    EntityManager &entityManager;
    SceneManager &sceneManager;
    TextureManager &textureManager;
    AudioManager &audioManager;
    InputManager &inputManager;
    sf::RenderWindow &window;

    Scene(
        SceneManager &sceneManager, EntityManager &entityManager,
        TextureManager &textureManager, AudioManager &audioManager,
        InputManager &inputManager, sf::RenderWindow &window
    )
        : sceneManager(sceneManager), entityManager(entityManager),
          textureManager(textureManager), audioManager(audioManager),
          inputManager(inputManager), window(window) {
    }
};
