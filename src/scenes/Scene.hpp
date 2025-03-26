#pragma once

#include <SFML/System/Time.hpp>

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
};
