#include "Scene.hpp"

#include "../managers/AudioManager.hpp"
#include "../managers/EntityManager.hpp"
#include "../managers/SceneManager.hpp"
#include "../managers/TextureManager.hpp"
#include <SFML/Window.hpp>

// Scena, ki je pred samim začetkom igre
// Ni enaka sceni, ki prikazuje začetni meni
class PreGameScene : public Scene {
  public:
    PreGameScene(
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
    void sInput() override;
    void sMovement(const sf::Time dt) override;
    void sAnimation() override;
    void sRender() override;

    // Shranjuje, v katero smer se premika igralec
    bool m_up = true;
    // Koliko sličic je že bilo
    unsigned int m_sceneFrame = 0;

    // Shranjuje čas, ki omejuje, kdaj lahko uporabnik začne z igro
    float m_timer = 0.f;
};
