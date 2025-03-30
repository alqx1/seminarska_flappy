#include "../managers/AudioManager.hpp"
#include "../managers/EntityManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"

// Prvi/začetni meni, s katerimi se uporabnik
// sreča, ko zažene program
class MainMenuScene : public Scene {
  public:
    MainMenuScene(
        SceneManager &sceneManager, EntityManager &entityManager,
        TextureManager &textureManager, AudioManager &audioManager,
        InputManager &inputManager, sf::RenderWindow &window
    );

    void init() override;
    void run(const sf::Time dt) override;

    void onActivate() override;
    void onDeactivate() override;

  private:
    // Sistemi
    void sInput() override;
    void sMovement(const sf::Time dt) override;
    void sCollision() override;
    void sAnimation() override;
    void sRender() override;

    // Igra naredi pred-igro sceno in igralno sceno ter
    // prestavi delovanje na pred-igra sceno
    void startGame();

    // Gumba, na katera lahko uporabnik pritisne
    std::shared_ptr<Entity> start;
    std::shared_ptr<Entity> exit;

    // Shranjuje, v katero smer se premika igralec
    bool up = true;
    unsigned int m_sceneFrame = 0;
};
