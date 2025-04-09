#include "GameOverScene.hpp"
#include "../Constants.hpp"

// Ustvajanje zapisa točk
static std::vector<unsigned int>
convertToIndividualNums(const unsigned int num);
static TextureTag getScoreNumberTag(const unsigned int num);
static void makeScore(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, const unsigned int score, const int y
);

static void makeMenu(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, const unsigned int best, const unsigned int score
);
static void makeButtons(
    std::shared_ptr<Entity> &reset, std::shared_ptr<Entity> &exit,
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
);

GameOverScene::GameOverScene(
    SceneManager &sceneManager, EntityManager &entityManager,
    TextureManager &textureManager, AudioManager &audioManager,
    InputManager &inputManager, sf::RenderWindow &window,
    const unsigned int score, const unsigned int best
)
    : Scene(
          sceneManager, entityManager, textureManager, audioManager,
          inputManager, window
      ),
      m_score(score), m_best(best) {
}


void GameOverScene::init() {
    makeMenu(entityManager, textureManager, window, m_best, m_score);
    makeButtons(reset, exit, entityManager, textureManager, window);

    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        // Moramo postaviti igralca ven iz
        // tal, da lahko skoči
        auto yHalf = bird->m_collisionShape->shape.getSize().y / 2;
        if (bird->m_cTransform->pos.y + yHalf >=
            window.getSize().y - FLOORHEIGHT) {
            bird->m_cTransform->pos.y =
                window.getSize().y - FLOORHEIGHT - yHalf;
        }

        // Skok ptice
        bird->m_cTransform->velocity.y = -JUMPVEL;
        bird->m_cTransform->angle = -30;
    }
}

void GameOverScene::onDeactivate() {
    // Odstrani vse elemente, ki jih naslednja scena ne potrebuje
    for (auto &menu : entityManager.getEntities(EntityTag::menu)) {
        menu->kill();
    }
    for (auto &text : entityManager.getEntities(EntityTag::text)) {
        text->kill();
    }
    for (auto &button : entityManager.getEntities(EntityTag::button)) {
        button->kill();
    }
    for (auto &pipe : entityManager.getEntities(EntityTag::pipe)) {
        pipe->kill();
    }
    for (auto &scoreNum : entityManager.getEntities(EntityTag::scoreNumber)) {
        scoreNum->kill();
    }
    entityManager.update();
}

void GameOverScene::sInput() {
    // Če uporabnik pretisne preslednico, ponovi igro
    if (inputManager.getInputStatus(sf::Keyboard::Space)) {
        sceneManager.switchTo("pre");
    }

    // Če pritisne Q, ugasni
    if (inputManager.getInputStatus(sf::Keyboard::Q)) {
        sceneManager.turnOffGame();
    }
}

void GameOverScene::sPhysics() {
    // Skrbi za padanje ptice
    const float dt = 1.0 / 60;
    const float factor = 10;
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        if (bird->m_cTransform) {
            bird->m_cTransform->velocity.y += GRAVITY * dt * factor;
        }
    }
}

void GameOverScene::sMovement(const sf::Time dt) {
    // Če ptica še pada, poskrbi za padanje in njeno premikanje/obračanje
    if (m_gravityOn) {
        for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
            if (!bird->m_cTransform) {
                continue;
            }
            bird->m_cTransform->pos +=
                bird->m_cTransform->velocity * dt.asSeconds();

            if (bird->m_collisionShape) {
                bird->m_collisionShape->shape.setPosition(
                    bird->m_cTransform->pos
                );
            }

            if (bird->m_sprite) {
                bird->m_sprite->setPosition(bird->m_cTransform->pos);
            }

            if (bird->m_cTransform->velocity.y > 0 &&
                bird->m_cTransform->angle < 70) {
                m_falling += dt.asSeconds();
                if (m_falling >= 0.1f) {
                    bird->m_cTransform->angle += GRAVITY * 5.f * dt.asSeconds();
                }
            }

            bird->m_sprite->setRotation(bird->m_cTransform->angle);
        }
    }
}

void GameOverScene::sCollision() {
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        // Če se ptica dotakne tal, prenehaj s padanjem
        auto yHalf = bird->m_collisionShape->shape.getSize().y / 2;
        if (bird->m_cTransform->pos.y + yHalf >
            window.getSize().y - FLOORHEIGHT) {
            m_gravityOn = false;
        }
    }

    // Če uporabnik klikne na gumb, izvede svojo funkcijo
    if (reset->m_collisionShape->isInside(inputManager.getMousePos()) &&
        inputManager.getMouseStatus(sf::Mouse::Left)) {
        sceneManager.switchTo("pre");
    }

    if (exit->m_collisionShape->isInside(inputManager.getMousePos()) &&
        inputManager.getMouseStatus(sf::Mouse::Left)) {
        sceneManager.turnOffGame();
    }
}

void GameOverScene::sRender() {
    // Prikaže potrebne entitete
    for (auto &bg : entityManager.getEntities(EntityTag::background)) {
        if (bg->m_sprite) {
            window.draw(*bg->m_sprite);
        }
    }

    for (auto &pipe : entityManager.getEntities(EntityTag::pipe)) {
        if (pipe->m_sprite) {
            window.draw(*pipe->m_sprite);
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

    for (auto &menu : entityManager.getEntities(EntityTag::menu)) {
        if (menu->m_sprite) {
            window.draw(*menu->m_sprite);
        }
    }

    for (auto &scoreNum : entityManager.getEntities(EntityTag::scoreNumber)) {
        if (scoreNum->m_sprite) {
            window.draw(*scoreNum->m_sprite);
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

void GameOverScene::run(const sf::Time dt) {
    sInput();
    if (m_gravityOn) {
        sPhysics();
    }
    sMovement(dt);
    sCollision();
    sRender();
}

// Pridobi vektor števk števila
std::vector<unsigned int> convertToIndividualNums(const unsigned int num) {
    std::vector<unsigned int> nums;
    unsigned int i = num;
    do {
        nums.emplace_back(i % 10);
        i /= 10;
    } while (i > 0);

    std::reverse(nums.begin(), nums.end());

    return nums;
}

// Vrne teksturo števke glede na števko
TextureTag getScoreNumberTag(const unsigned int num) {
    return (TextureTag)(num + t0);
}

// Ustvari števila za rezultat
// Delovanje je enako kot v igrni sceni (GameScene.cpp)
// le, da so števila manjša
void makeScore(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, const unsigned int score, const int y
) {
    std::vector<unsigned int> nums = convertToIndividualNums(score);
    const unsigned int numsSize = nums.size();

    unsigned int numberScale = SCALE * 0.5f;

    unsigned int positionFactor =
        24 * numberScale;

    for (int i = 0; i < numsSize; i++) {
        auto number = entityManager.addEntity(EntityTag::scoreNumber);
        number->m_sprite = std::make_shared<sf::Sprite>(
            textureManager.getTexture(getScoreNumberTag(nums[i]))
        );
        number->m_sprite->setPosition(
            window.getSize().x / 2.f - (positionFactor * numsSize / 2.f) +
                positionFactor * i,
            y
        );
        number->m_sprite->setScale(numberScale, numberScale);
    }
}

void makeMenu(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, const unsigned int best, const unsigned int score
) {
    // Ustvarimo ozadje menija
    auto menu = entityManager.addEntity(EntityTag::menu);

    // Priredimo sliko ozadju menija
    menu->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tGameOver));
    auto menubb = menu->m_sprite->getLocalBounds();
    menu->m_sprite->setScale(SCALE * 1.5f, SCALE * 1.5f);
    menu->m_sprite->setOrigin(menubb.width / 2.f, menubb.height / 2.f);
    menu->m_sprite->setPosition(
        window.getSize().x / 2.f, window.getSize().y / 2.f
    );

    // Ustvarimo končni tekst
    auto text = entityManager.addEntity(EntityTag::text);
    text->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tGameOverText));
    text->m_sprite->setScale(SCALE, SCALE);
    auto textbb = text->m_sprite->getLocalBounds();
    text->m_sprite->setOrigin(textbb.width / 2.f, textbb.height / 2.f);
    text->m_sprite->setPosition(
        window.getSize().x / 2.f, menu->m_sprite->getPosition().y -
                                      (menubb.height * 1.5f) - 50.f -
                                      textbb.height * SCALE
    );

    // Ustvarimo rezultat prejšne igre in najboljše igre
    makeScore(
        entityManager, textureManager, window, score,
        menu->m_sprite->getPosition().y -
            (menubb.height / 2.f - 15) * SCALE * 1.5f
    );
    makeScore(
        entityManager, textureManager, window, best,
        menu->m_sprite->getPosition().y +
            (menubb.height / 2.f - 24) * SCALE * 1.5f
    );
}

void makeButtons(
    std::shared_ptr<Entity> &reset, std::shared_ptr<Entity> &exit,
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window
) {
    // Ustvarimo gumba za resetiranje in izhod
    reset = entityManager.addEntity(EntityTag::button);
    exit = entityManager.addEntity(EntityTag::button);

    sf::Vector2f middlePoint(
        window.getSize().x / 2.f,
        window.getSize().y / 2.f + window.getSize().y / 4.f
    );
    float padding = 20;
    sf::Vector2f buttonSize(80 * SCALE, 20 * SCALE);

    // Določimo njuno pozicijo
    // Centrirana sta glede na sredino
    reset->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(0, 0),
        sf::Vector2f(
            middlePoint.x - padding - buttonSize.x,
            middlePoint.y - buttonSize.y / 2.f
        ),
        0.f
    );
    exit->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(0, 0),
        sf::Vector2f(
            middlePoint.x + padding, middlePoint.y - buttonSize.y / 2.f
        ),
        0.f
    );

    // Priredimo slike
    reset->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tButtons));
    reset->m_sprite->setTextureRect(sf::IntRect(0, 40, 80, 20));
    reset->m_sprite->setPosition(reset->m_cTransform->pos);
    reset->m_sprite->setScale(SCALE, SCALE);

    exit->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tButtons));
    exit->m_sprite->setTextureRect(sf::IntRect(0, 20, 80, 20));
    exit->m_sprite->setPosition(exit->m_cTransform->pos);
    exit->m_sprite->setScale(SCALE, SCALE);

    // Ustvarimo kolizijske like
    reset->m_collisionShape = std::make_shared<CRectangle>(buttonSize);
    exit->m_collisionShape = std::make_shared<CRectangle>(buttonSize);
    reset->m_collisionShape->shape.setPosition(reset->m_cTransform->pos);
    exit->m_collisionShape->shape.setPosition(exit->m_cTransform->pos);
}
