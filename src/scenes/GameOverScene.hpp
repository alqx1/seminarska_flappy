#pragma once

#include "../managers/AudioManager.hpp"
#include "../managers/EntityManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"
#include "Scene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class GameOverScene : public Scene {
  public:
    GameOverScene(
        SceneManager &sceneManager, EntityManager &entityManager,
        TextureManager &textureManager, AudioManager &AudioManager,
        sf::RenderWindow &window, const unsigned int score,
        const unsigned int best
    );
    void init() override;
    void run(const sf::Time dt) override;

    void onDeactivate() override;

  private:
    void sInput() override;
    void sPhysics() override;
    void sMovement(const sf::Time dt) override;
    void sCollision() override;
    void sRender() override;

    bool m_gravityOn = true;
    float m_falling = 0.f;
    unsigned int m_score;
    unsigned int m_best;

    std::shared_ptr<Entity> reset;
    std::shared_ptr<Entity> exit;
};
