#pragma once

#include "../managers/EntityManager.hpp"
#include "Scene.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class GameOverScene : public Scene {
  private:
    void sInput() override;
    void sPhysics() override;
    void sMovement(const sf::Time dt) override;
    void sCollision() override;
    void sRender() override;

    SceneManager &sceneManager;
    EntityManager &entityManager;
    TextureManager &textureManager;
    sf::RenderWindow &window;

    std::vector<unsigned int> convertToIndividualNums(const unsigned int num);
    TextureTag getScoreNumberTag(const unsigned int num);
    void makeScore(const unsigned int score, const int y);
    void makeMenu();
    void makeButtons();

    bool m_gravityOn = true;
    float m_falling = 0.f;
    unsigned int m_score;
    unsigned int m_best;

    std::shared_ptr<Entity> reset;
    std::shared_ptr<Entity> exit;

  public:
    GameOverScene(
        SceneManager &sceneManager, EntityManager &entityManager,
        TextureManager &textureManager, sf::RenderWindow &window,
        const unsigned int score, const unsigned int best);
    void init() override;
    void run(const sf::Time dt) override;

    void onActivate() override;
    void onDeactivate() override;
};
