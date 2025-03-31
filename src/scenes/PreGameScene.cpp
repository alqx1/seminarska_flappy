#include "PreGameScene.hpp"
#include "../Constants.hpp"

// Resetira glavnega igralca
static void resetBird(std::shared_ptr<Entity> bird, sf::Window &window);

// Ustvari zapis, ki ga uporabnik sreča pred začenjanjem igranja
static void makeMessage(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);

PreGameScene::PreGameScene(
    SceneManager &sceneManager, EntityManager &entityManager,
    TextureManager &textureManager, AudioManager &audioManager,
    InputManager &inputManager, sf::RenderWindow &window
)
    : Scene(
          sceneManager, entityManager, textureManager, audioManager,
          inputManager, window
      ) {
}

void PreGameScene::run(const sf::Time dt) {
    sInput();
    sMovement(dt);
    sAnimation();
    sRender();
    m_sceneFrame++;
    m_timer += dt.asSeconds();
}

void PreGameScene::init() {
}

void PreGameScene::onActivate() {
    m_timer = 0.f;
    m_up = true;

    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        resetBird(bird, window);
    }
    makeMessage(entityManager, textureManager, window);
}

void PreGameScene::onDeactivate() {
    for (auto &text : entityManager.getEntities(EntityTag::text)) {
        text->kill();
    }
    entityManager.update();
}

void PreGameScene::sInput() {
    // Če je že dovolj časa minilo, lahko uporabnik začne
    // igro s levim klikom ali s preslednico
    if (m_timer > 1.f && (inputManager.getInputStatus(sf::Keyboard::Space) ||
                          inputManager.getMouseStatus(sf::Mouse::Left))) {
        // game scene was already made before,
        // so we can just switch to it now
        sceneManager.switchTo("game");
    }
}

void PreGameScene::sMovement(const sf::Time dt) {
    // Poskrbi za začetno premikanje ptice/igralca,
    // ki se pomika gor in dol na začetku
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        if (m_up) {
            if (bird->m_cTransform->velocity.y >= 0) {
                m_up = false;
                bird->m_cTransform->velocity.y = MENUANIMACCEL / 2.f;
            }
            bird->m_cTransform->velocity.y += MENUANIMACCEL * dt.asSeconds();
        }
        else {
            if (bird->m_cTransform->velocity.y <= 0) {
                m_up = true;
                bird->m_cTransform->velocity.y = -MENUANIMACCEL / 2.f;
            }
            bird->m_cTransform->velocity.y -= MENUANIMACCEL * dt.asSeconds();
        }

        bird->m_cTransform->pos.y +=
            bird->m_cTransform->velocity.y * dt.asSeconds();
        bird->m_cTransform->pos.x +=
            bird->m_cTransform->velocity.x * dt.asSeconds();
        bird->m_sprite->setPosition(bird->m_cTransform->pos);
        bird->m_collisionShape->shape.setPosition(bird->m_cTransform->pos);
    }
}

void PreGameScene::sAnimation() {
    // Ureja hitrost animacije.
    if (m_sceneFrame % ANIMSPEED != 0) {
        return;
    }

    // Dobi, na kateri sličici animacije je in nato
    // spremeni na naslednjo, ki mora biti
    int animFrame = (m_sceneFrame / ANIMSPEED) % 4;
    switch (animFrame) {
    case 0: {
        for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
            bird->m_sprite->setTexture(textureManager.getTexture(tBird0));
        }
        break;
    }

    case 1: {
        for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
            bird->m_sprite->setTexture(textureManager.getTexture(tBird1));
        }
        break;
    }

    case 2: {
        for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
            bird->m_sprite->setTexture(textureManager.getTexture(tBird2));
        }
        break;
    }

    case 3: {
        for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
            bird->m_sprite->setTexture(textureManager.getTexture(tBird1));
        }
        break;
    }
    }
}

void PreGameScene::sRender() {
    // Nariše ozadje, spodnja tla, ptico in zapis
    for (auto &bg : entityManager.getEntities(EntityTag::background)) {
        if (bg->m_sprite) {
            window.draw(*bg->m_sprite);
        }
    }

    for (auto &base : entityManager.getEntities(EntityTag::base)) {
        if (base->m_sprite) {
            window.draw(*base->m_sprite);
        }
    }

    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        if (bird->m_sprite) {
            window.draw(*bird->m_sprite);
        }
    }

    for (auto &text : entityManager.getEntities(EntityTag::text)) {
        if (text->m_sprite) {
            window.draw(*text->m_sprite);
        }
    }
}

void resetBird(std::shared_ptr<Entity> bird, sf::Window &window) {
    bird->m_cTransform->angle = 0;
    bird->m_sprite->setRotation(0);
    bird->m_cTransform->velocity = {0, 0};
    bird->m_cTransform->pos = {
        window.getSize().x / 2.f -
            window.getSize().x / 10.f, // Ptica je levo od sredine
        (window.getSize().y - FLOORHEIGHT) / 2.f
    };
}

void makeMessage(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
) {
    // Ustvari entiteto napis in jo premakne
    // na določen položaj, odvisen od velikosti okna
    auto msg = entityManager.addEntity(EntityTag::text);
    msg->m_sprite = std::make_shared<sf::Sprite>();
    msg->m_sprite->setTexture(textureManager.getTexture(TextureTag::tPreMessage)
    );
    sf::FloatRect sb = msg->m_sprite->getLocalBounds();
    msg->m_sprite->setOrigin(sb.width / 2.f, sb.height / 2.f);
    msg->m_sprite->setScale(SCALE, SCALE);
    msg->m_sprite->setPosition(
        window.getSize().x / 2.f, window.getSize().y / 5.f
    );
}
