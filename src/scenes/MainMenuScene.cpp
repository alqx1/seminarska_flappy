#include "MainMenuScene.hpp"
#include "../Constants.hpp"
#include "GameScene.hpp"
#include "PreGameScene.hpp"

// Funkcije za ustvarjanje raznih entitet
static void makeMenu(
    std::shared_ptr<Entity> &exit, std::shared_ptr<Entity> &start,
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);
static void makeBird(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);
static void makeBase(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);
static void makeBackground(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);

MainMenuScene::MainMenuScene(
    SceneManager &sceneManager, EntityManager &entityManager,
    TextureManager &textureManager, AudioManager &audioManager,
    InputManager &inputManager, sf::RenderWindow &window
)
    : Scene(
          sceneManager, entityManager, textureManager, audioManager,
          inputManager, window
      ) {
}

void MainMenuScene::init() {
    // Ustvari tla in ozadje
    makeBase(entityManager, textureManager, window);
    makeBackground(entityManager, textureManager, window);
}

void MainMenuScene::run(const sf::Time dt) {
    sInput();
    sMovement(dt);
    sCollision();
    sAnimation();
    sRender();
    m_sceneFrame++;
}

void MainMenuScene::onActivate() {
    // Ustvari ptico/igralca in meni
    makeBird(entityManager, textureManager, window);
    makeMenu(exit, start, entityManager, textureManager, window);
}

void MainMenuScene::onDeactivate() {
    // Odstrani meni in nastali tekst
    for (auto &button : entityManager.getEntities(button)) {
        button->kill();
    }

    for (auto &text : entityManager.getEntities(text)) {
        text->kill();
    }
}

void MainMenuScene::startGame() {
    // already makes game scene here
    sceneManager.add(
        "pre", std::make_shared<PreGameScene>(
                   sceneManager, entityManager, textureManager, audioManager,
                   inputManager, window
               )
    );
    sceneManager.add(
        "game", std::make_shared<GameScene>(
                    sceneManager, entityManager, textureManager, audioManager,
                    inputManager, window
                )
    );
    sceneManager.switchTo("pre");
}

void MainMenuScene::sInput() {
    if (inputManager.getInputStatus(sf::Keyboard::Space)) {
        startGame();
    }

    if (inputManager.getInputStatus(sf::Keyboard::Q)) {
        sceneManager.turnOffGame();
    }
}

void MainMenuScene::sMovement(const sf::Time dt) {
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        // bobing motion function
        // if bird moving up, then slowly add gravity
        if (up) {
            if (bird->m_cTransform->velocity.y >= 0) {
                up = false;
                bird->m_cTransform->velocity.y = MENUANIMACCEL / 2.f;
            }
            bird->m_cTransform->velocity.y += MENUANIMACCEL * dt.asSeconds();
        }
        // else take it away
        else {
            if (bird->m_cTransform->velocity.y <= 0) {
                up = true;
                bird->m_cTransform->velocity.y = -MENUANIMACCEL / 2.f;
            }
            bird->m_cTransform->velocity.y -= MENUANIMACCEL * dt.asSeconds();
        }

        // adds velocity and sets positions
        bird->m_cTransform->pos.y +=
            bird->m_cTransform->velocity.y * dt.asSeconds();
        bird->m_sprite->setPosition(bird->m_cTransform->pos);
    }
}

void MainMenuScene::sCollision() {
    if (inputManager.getMouseStatus(sf::Mouse::Left)) {
        // if mouse down, check for collisions
        if (start->m_collisionShape->isInside(inputManager.getMousePos())) {
            startGame();
        }

        if (exit->m_collisionShape->isInside(inputManager.getMousePos())) {
            sceneManager.turnOffGame();
        }
    }
}

void MainMenuScene::sAnimation() {
    // handles animations

    // if its not time to switch animation, return
    if (m_sceneFrame % ANIMSPEED != 0) {
        return;
    }

    // gets animation frame
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

void MainMenuScene::sRender() {
    // renders needed entities
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

    for (auto &button : entityManager.getEntities(EntityTag::button)) {
        if (button->m_sprite) {
            window.draw(*button->m_sprite);
        }
    }
}

void makeMenu(
    std::shared_ptr<Entity> &exit, std::shared_ptr<Entity> &start,
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
) {
    // Ustvarjanje gumbov za začetek in izhod
    exit = entityManager.addEntity(EntityTag::button);
    start = entityManager.addEntity(EntityTag::button);

    // Sredinska točka med gumboma
    sf::Vector2f middlePoint(
        window.getSize().x / 2.f, window.getSize().y / 2.f + 180.f
    );

    // Razdalja gumba od sredinske točke
    float padding = 20;
    sf::Vector2f buttonSize(80 * SCALE, 20 * SCALE);

    // Nastavljanje položaja gumbov
    start->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(0, 0),
        sf::Vector2f(
            middlePoint.x - padding - buttonSize.x, // Levo od točke
            middlePoint.y
        ), // Poravnano s sredinsko točko
        0.f
    );
    exit->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(0, 0),
        sf::Vector2f(middlePoint.x + padding, middlePoint.y), 0.f
    );

    // Pripravljanje slik za gumbe
    start->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tButtons));
    start->m_sprite->setTextureRect(sf::IntRect(0, 0, 80, 20));
    start->m_sprite->setPosition(start->m_cTransform->pos);
    start->m_sprite->setScale(SCALE, SCALE);

    exit->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tButtons));
    exit->m_sprite->setTextureRect(sf::IntRect(0, 20, 80, 20));
    exit->m_sprite->setPosition(exit->m_cTransform->pos);
    exit->m_sprite->setScale(SCALE, SCALE);

    // Dodajanje območji, na katere lahko uporabnik pritisne
    start->m_collisionShape = std::make_shared<CRectangle>(buttonSize);
    exit->m_collisionShape = std::make_shared<CRectangle>(buttonSize);
    start->m_collisionShape->shape.setPosition(start->m_cTransform->pos);
    exit->m_collisionShape->shape.setPosition(exit->m_cTransform->pos);

    // Ustvarjanje začetnega napisa ter nastavljanje njegovega položaja
    auto welcome = entityManager.addEntity(EntityTag::text);
    welcome->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tWelcomeText));
    auto bb = welcome->m_sprite->getLocalBounds();
    welcome->m_sprite->setOrigin(bb.width / 2, bb.height / 2);
    welcome->m_sprite->setScale(SCALE, SCALE);
    welcome->m_sprite->setPosition(
        sf::Vector2f(
            window.getSize().x / 2.f,
            window.getSize().y / 2.f - window.getSize().y / 3.f
        )
    );
}

void makeBird(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
) {
    // Ustvari entiteto
    auto bird = entityManager.addEntity(EntityTag::bird);

    // Nastavljanje njenega položaja
    bird->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(0.f, GRAVITY),
        sf::Vector2f(
            window.getSize().x / 2.f -
                window.getSize().x / 10.f, // Ptica je levo od sredine
            (window.getSize().y - FLOORHEIGHT) / 2.f
        ), // Sredina okna brez upoštevanja tal
        0.f
    );

    // Dodajanje komponente za vnos
    bird->m_cInput = std::make_shared<CInput>();

    // Nastavljanje slike za ptico
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

void makeBase(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
) {
    // Naredi tla (slika, kolizijski pravokotnik, nastavi položaj)
    auto base = entityManager.addEntity(EntityTag::base);

    base->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(-PIPESPEED, 0),
        sf::Vector2f(0, window.getSize().y - FLOORHEIGHT), 0.f
    );

    base->m_collisionShape =
        std::make_shared<CRectangle>(sf::Vector2f(336 * SCALE, 112 * SCALE));

    base->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tBase));
    base->m_sprite->setScale(SCALE, SCALE);
    base->m_sprite->setPosition(base->m_cTransform->pos);
}

void makeBackground(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
) {
    // makes background
    auto bg = entityManager.addEntity(EntityTag::background);
    bg->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(0, 0), sf::Vector2f(0, window.getSize().y), 0.f
    );

    bg->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tBackground));
    bg->m_sprite->setScale(SCALE, SCALE);
    sf::FloatRect bb = bg->m_sprite->getLocalBounds();
    bg->m_sprite->setOrigin(0, bb.height);
    bg->m_sprite->setPosition(bg->m_cTransform->pos);
}
