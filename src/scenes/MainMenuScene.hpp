#include "../managers/AudioManager.hpp"
#include "../managers/EntityManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"
#include "Scene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

// Prvi/začetni meni, s katerimi se uporabnik
// sreča, ko zažene program
class MainMenuScene : public Scene {
  private:
    SceneManager &sceneManager;
    EntityManager &entityManager;
    TextureManager &textureManager;
    AudioManager &audioManager;
    sf::RenderWindow &window;

    // Gumba, na katera lahko uporabnik pritisne
    std::shared_ptr<Entity> start;
    std::shared_ptr<Entity> exit;

    // Funkcije za ustvarjanje raznih entitet
    void makeMenu();
    void makeBird();
    void makeBase();
    void makeBackground();

    // Igra naredi pred-igro sceno in igralno sceno
    void startGame();

    // Sistemi
    void sInput() override;
    void sMovement(const sf::Time dt) override;
    void sCollision() override;
    void sAnimation() override;
    void sRender() override;

    // Shranjuje, v katero smer se premika igralec
    bool up = true;
    unsigned int m_sceneFrame = 0;

  public:
    MainMenuScene(
        SceneManager &sceneManager, EntityManager &entityManager,
        TextureManager &textureManager, AudioManager &audioManager,
        sf::RenderWindow &window
    );

    void init() override;
    void run(const sf::Time dt) override;

    void onActivate() override;
    void onDeactivate() override;
};
