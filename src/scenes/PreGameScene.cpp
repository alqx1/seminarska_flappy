#include "PreGameScene.hpp"
#include "Constants.hpp"

// Ustvari glavnega igralca
static void makeBird(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);

// Ustvari zapis, ki ga uporabnik sreča pred začenjanjem igranja
static void makeMessage(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);

PreGameScene::PreGameScene(
    SceneManager &sceneManager, EntityManager &entityManager,
    TextureManager &textureManager, AudioManager &audioManager,
    sf::RenderWindow &window
)
    : Scene(sceneManager, entityManager, textureManager, audioManager, window) {
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

    for (auto &pipe : entityManager.getEntities(EntityTag::pipe)) {
        pipe->kill();
    }
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        bird->kill();
    }
    for (auto &text : entityManager.getEntities(EntityTag::text)) {
        text->kill();
    }
    for (auto &scoreBox : entityManager.getEntities(EntityTag::scoreBox)) {
        scoreBox->kill();
    }
    for (auto &scoreNum : entityManager.getEntities(EntityTag::scoreNumber)) {
        scoreNum->kill();
    }
    entityManager.update();

    makeBird(entityManager, textureManager, window);
    makeMessage(entityManager, textureManager, window);
}

void PreGameScene::onDeactivate() {
    for (auto &text : entityManager.getEntities(EntityTag::text)) {
        text->kill();
    }
    for (auto &scoreBox : entityManager.getEntities(EntityTag::scoreBox)) {
        scoreBox->kill();
    }
    for (auto &scoreNum : entityManager.getEntities(EntityTag::scoreNumber)) {
        scoreNum->kill();
    }
}

void PreGameScene::sInput() {
    // Če je že dovolj časa minilo, lahko uporabnik začne
    // igro s levim klikom ali s preslednico
    if (m_timer > 1.f && (sceneManager.getInputStatus(sf::Keyboard::Space) ||
                          sceneManager.getMouseStatus(sf::Mouse::Left))) {
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
                bird->m_cTransform->velocity.y = GRAVITY / 2.f;
            }
            bird->m_cTransform->velocity.y += GRAVITY * dt.asSeconds();
        }
        else {
            if (bird->m_cTransform->velocity.y <= 0) {
                m_up = true;
                bird->m_cTransform->velocity.y = -GRAVITY / 2.f;
            }
            bird->m_cTransform->velocity.y -= GRAVITY * dt.asSeconds();
        }

        bird->m_cTransform->pos.y +=
            bird->m_cTransform->velocity.y * dt.asSeconds();
        bird->m_sprite->setPosition(
            sf::Vector2f(bird->m_cTransform->pos.x, bird->m_cTransform->pos.y)
        );
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

void makeBird(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
) {
    auto bird = entityManager.addEntity(EntityTag::bird);

    bird->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(0.f, GRAVITY),
        sf::Vector2f(
            window.getSize().x / 2.f - window.getSize().x / 10.f,
            window.getSize().y / 2.f - window.getSize().y / 10.f
        ),
        0.f
    );

    bird->m_cInput = std::make_shared<CInput>();

    bird->m_sprite = std::make_shared<sf::Sprite>();
    bird->m_sprite->setTexture(textureManager.getTexture(tBird0));
    sf::FloatRect sb = bird->m_sprite->getLocalBounds();
    bird->m_sprite->setOrigin(sb.width / 2.f, sb.height / 2.f);
    bird->m_sprite->setScale(SCALE, SCALE);
    bird->m_sprite->setPosition(bird->m_cTransform->pos);

    bird->m_collisionShape = std::make_shared<CRectangle>(
        sf::Vector2f(sb.width * 0.75f * SCALE, sb.height * 0.75f * SCALE)
    );
    bird->m_collisionShape->shape.setOrigin(
        bird->m_collisionShape->shape.getSize().x / 2,
        bird->m_collisionShape->shape.getSize().y / 2
    );
    bird->m_collisionShape->shape.setPosition(bird->m_cTransform->pos);
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
