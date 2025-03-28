#include "../managers/AudioManager.hpp"
#include "../managers/EntityManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"
#include "Scene.hpp"

class GameScene : public Scene {
  public:
    GameScene(
        SceneManager &sceneManager, EntityManager &entityManager,
        TextureManager &textureManager, AudioManager &audioManager,
        sf::RenderWindow &window
    );

    void init() override;
    void run(const sf::Time dt) override;

    void onActivate() override;
    void onDeactivate() override;

  private:
    // Sistemi
    void sSpawner(const sf::Time dt) override;
    void sInput() override;
    void sPhysics() override;
    void sMovement(const sf::Time dt) override;
    void sCollision() override;
    void sAnimation() override;
    void sRender() override;

    // Ustvajanje zapisa točk
    // Naredi par pip, eno zgoraj, drugo spodaj
    void makePipePair();

    // Preide na naslednjo končno sceno
    void gameover();

    float m_falling = 0.f; // Za premikanje ptice okoli težišča gor in dol
    unsigned int m_sceneFrame = 0;
    float m_pipeTimer; // Za ustvarjanje pip
    // Med delovanjem shranjujemo točke in najboljše točke dosežene
    unsigned int m_score = 0;
    unsigned int m_best = 0;
};
