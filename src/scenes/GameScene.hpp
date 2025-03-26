#include "Scene.hpp"

#include "../managers/AudioManager.hpp"
#include "../managers/EntityManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <vector>

class GameScene : public Scene {
  public:
    GameScene(
        SceneManager &sceneManager, EntityManager &entityManager,
        TextureManager &textureManager, AudioManager &audioManager,
        sf::RenderWindow &window);

    void init() override;
    void run(const sf::Time dt) override;

    void onActivate() override;
    void onDeactivate() override;

  private:
    EntityManager &entityManager;
    SceneManager &sceneManager;
    TextureManager &textureManager;
    AudioManager &audioManager;
    sf::RenderWindow &window;

    float m_falling = 0.f; // Za premikanje ptice okoli težišča gor in dol
    unsigned int m_sceneFrame = 0;
    float m_pipeTimer; // Za ustvarjanje pip
    // Med delovanjem shranjujemo točke in najboljše točke dosežene
    unsigned int m_score = 0;
    unsigned int m_best = 0;

    // Ustvajanje zapisa točk
    std::vector<unsigned int> convertToIndividualNums(const unsigned int num);
    TextureTag getScoreNumberTag(const unsigned int num);
    void makeScore();
    // Naredi par pip, eno zgoraj, drugo spodaj
    void makePipePair();

    // Preide na naslednjo končno sceno
    void gameover();

    // Sistemi
    void sSpawner(const sf::Time dt) override;
    void sInput() override;
    void sPhysics() override;
    void sMovement(const sf::Time dt) override;
    void sCollision() override;
    void sAnimation() override;
    void sRender() override;
};
