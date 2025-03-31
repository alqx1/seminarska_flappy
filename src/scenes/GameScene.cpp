#include "GameScene.hpp"
#include "../Constants.hpp"
#include "GameOverScene.hpp"

// Ustvajanje zapisa točk
static std::vector<unsigned int>
convertToIndividualNums(const unsigned int num);
static TextureTag getScoreNumberTag(const unsigned int num);
static void makeScore(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, unsigned int score
);

// Naredi par pip, eno zgoraj, drugo spodaj
static void makePipePair(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, unsigned int score
);

GameScene::GameScene(
    SceneManager &sceneManager, EntityManager &entityManager,
    TextureManager &textureManager, AudioManager &audioManager,
    InputManager &inputManager, sf::RenderWindow &window
)
    : Scene(
          sceneManager, entityManager, textureManager, audioManager,
          inputManager, window
      ) {
}

void GameScene::run(const sf::Time dt) {
    sSpawner(dt);
    sInput();
    sPhysics();
    sMovement(dt);
    sCollision();
    sAnimation();
    sRender();
    m_sceneFrame++;
}

void GameScene::init() {
}

void GameScene::onActivate() {
    // Da se takoj pojavi pipa
    m_pipeTimer = PIPESPAWN;
    m_score = 0;

    makeScore(entityManager, textureManager, window, m_score);
}

void GameScene::onDeactivate() {
    // Odstrani vse kar ni potrebno za naslednjo sceno
    for (auto &scoreNum : entityManager.getEntities(EntityTag::scoreNumber)) {
        scoreNum->kill();
    }
    for (auto &scoreBox : entityManager.getEntities(EntityTag::scoreBox)) {
        scoreBox->kill();
    }
}

// handles pipes spawning in
void GameScene::sSpawner(const sf::Time dt) {
    m_pipeTimer += dt.asSeconds();
    if (PIPESPAWN <= m_pipeTimer) {
        makePipePair(entityManager, textureManager, window, m_score);
        m_pipeTimer = 0.f;
    }
}

void GameScene::sInput() {
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        // if bird hasnt yet jumped -> jump
        if ((inputManager.getInputStatus(sf::Keyboard::Space) ||
             inputManager.getMouseStatus(sf::Mouse::Left)) &&
            !bird->m_cInput->hasJumped) {
            bird->m_cInput->jump = true;
            bird->m_cInput->hasJumped = true;
            audioManager.play(SoundTag::sJump);
        }
        // if nothing is pressed, reset all bool values
        else if (!(inputManager.getInputStatus(sf::Keyboard::Space) ||
                   inputManager.getMouseStatus(sf::Mouse::Left))) {
            bird->m_cInput->jump = false;
            bird->m_cInput->hasJumped = false;
        }
        // if something is pressed, but has jumped before, dont jump
        else {
            bird->m_cInput->jump = false;
        }
    }
}

// handles bird falling
void GameScene::sPhysics() {
    const float dt = 1.0 / 60;
    const float factor = 10;
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        bird->m_cTransform->velocity.y += GRAVITY * dt * factor;
    }
}

void GameScene::sMovement(const sf::Time dt) {
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        // if bird jumping, jump
        if (bird->m_cInput->jump) {
            bird->m_cTransform->velocity.y = -JUMPVEL;
            bird->m_cTransform->angle = -30.f;
            m_falling = 0.f;
        }

        // handles rotation when falling
        if (bird->m_cTransform->velocity.y > 0 &&
            bird->m_cTransform->angle < 70) {
            m_falling += dt.asSeconds();
            if (m_falling >= 0.2f) {
                bird->m_cTransform->angle += GRAVITY * 3.f * dt.asSeconds();
            }
        }

        bird->m_sprite->setRotation(bird->m_cTransform->angle);
    }

    // moves floor
    for (auto &base : entityManager.getEntities(EntityTag::base)) {
        // resets floor pos when passed some point
        if (base->m_cTransform->pos.x <= -12 * 4 * SCALE) {
            base->m_cTransform->pos.x = 0;
        }
    }

    for (auto &entity : entityManager.getEntities()) {
        // adding velocity to every entity
        if (entity->m_cTransform) {
            entity->m_cTransform->pos.y +=
                entity->m_cTransform->velocity.y * dt.asSeconds();
            entity->m_cTransform->pos.x +=
                entity->m_cTransform->velocity.x * dt.asSeconds();
            if (entity->m_sprite) {
                entity->m_sprite->setPosition(entity->m_cTransform->pos);
            }
        }

        // setting position to collision shape
        if (entity->m_collisionShape && entity->m_cTransform) {
            entity->m_collisionShape->shape.setPosition(
                entity->m_cTransform->pos
            );
        }
    }
}

void GameScene::sCollision() {
    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        auto yHalf = bird->m_collisionShape->shape.getSize().y / 2;
        // dont let the bird get out of bounds
        if (bird->m_cTransform->pos.y - yHalf < -100) {
            bird->m_cTransform->pos.y = -100 + yHalf;
        }

        // if hitting floor
        if (bird->m_cTransform->pos.y + yHalf >
            window.getSize().y - FLOORHEIGHT) {
            gameover();
        }
    }

    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        // here for more concisness
        const auto birdShape = bird->m_collisionShape->shape.getSize();
        const auto birdPos = bird->m_collisionShape->shape.getPosition();
        for (auto &pipe : entityManager.getEntities(EntityTag::pipe)) {
            // checks collision with pipe and bird
            const auto pipeShape = pipe->m_collisionShape->shape.getSize();
            const auto pipePos = pipe->m_collisionShape->shape.getPosition();

            // basic aabb collision
            bool horizontal =
                pipePos.x - pipeShape.x / 2.f < birdPos.x + birdShape.x / 2.f &&
                birdPos.x - birdShape.x / 2.f < pipePos.x + pipeShape.x / 2.f;
            bool vertical =
                pipePos.y - pipeShape.y / 2.f < birdPos.y + birdShape.y / 2.f &&
                birdPos.y - birdShape.y / 2.f < pipePos.y + pipeShape.y / 2.f;

            // if collision occurs vertically and horizontally
            // that means the 2 rectangles are colliding
            if (vertical && horizontal) {
                gameover();
            }

            // delete pipe when out of bounds
            if (pipePos.x + pipeShape.x < 1) {
                pipe->kill();
            }
        }

        for (auto &scoreBox : entityManager.getEntities(EntityTag::scoreBox)) {
            // more concise
            const auto scoreShape = scoreBox->m_collisionShape->shape.getSize();
            const auto scorePos =
                scoreBox->m_collisionShape->shape.getPosition();

            // aabb same as before
            bool horizontal =
                scorePos.x - scoreShape.x / 2.f <
                    birdPos.x + birdShape.x / 2.f &&
                birdPos.x - birdShape.x / 2.f < scorePos.x + scoreShape.x / 2.f;
            bool vertical =
                scorePos.y - scoreShape.y / 2.f <
                    birdPos.y + birdShape.y / 2.f &&
                birdPos.y - birdShape.y / 2.f < scorePos.y + scoreShape.y / 2.f;

            // if colliding, score point and delete scorebox
            if (vertical && horizontal) {
                m_score++;
                scoreBox->kill();
                for (auto &scoreNum :
                     entityManager.getEntities(EntityTag::scoreNumber)) {
                    scoreNum->kill();
                }
                makeScore(entityManager, textureManager, window, m_score);
                audioManager.play(SoundTag::sScore);
            }
        }
    }
}

void GameScene::sAnimation() {
    // already explained in mainmenu and pregamemenu
    if (m_sceneFrame % ANIMSPEED != 0) {
        return;
    }

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

void GameScene::sRender() {
    // normal rendering
    for (auto &bg : entityManager.getEntities(EntityTag::background)) {
        if (bg->m_sprite) {
            window.draw(*bg->m_sprite);
        }
    }

    for (auto &pipe : entityManager.getEntities(EntityTag::pipe)) {
        if (pipe->m_sprite) {
            window.draw(*pipe->m_sprite);
            window.draw(pipe->m_collisionShape->shape);
        }
    }

    for (auto &base : entityManager.getEntities(EntityTag::base)) {
        if (base->m_sprite) {
            window.draw(*base->m_sprite);
        }
    }

    for (auto &scoreNum : entityManager.getEntities(EntityTag::scoreNumber)) {
        if (scoreNum->m_sprite) {
            window.draw(*scoreNum->m_sprite);
        }
    }

    for (auto &bird : entityManager.getEntities(EntityTag::bird)) {
        if (bird->m_sprite) {
            window.draw(*bird->m_sprite);
            window.draw(bird->m_collisionShape->shape);
        }
    }
}

// returns score numbers in vector
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

// returns what texture for number to use
TextureTag getScoreNumberTag(const unsigned int num) {
    return (TextureTag)(num + t0);
}

// makes score numbers
void makeScore(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, unsigned int score
) {
    std::vector<unsigned int> nums = convertToIndividualNums(score);
    const unsigned int numsSize = nums.size();

    unsigned int positionFactor = 24 * SCALE; // 24 is pixel width of sprite

    // pozabu zakaj to dela
    // something about handling positions for different
    // numbers of number textures
    for (int i = 0; i < numsSize; i++) {
        auto number = entityManager.addEntity(EntityTag::scoreNumber);
        number->m_sprite = std::make_shared<sf::Sprite>(
            textureManager.getTexture(getScoreNumberTag(nums[i]))
        );
        auto bb = number->m_sprite->getLocalBounds();
        number->m_sprite->setOrigin(bb.width / 2.f, bb.height / 2.f);
        number->m_sprite->setPosition(
            window.getSize().x / 2.f - (positionFactor * (numsSize - 1)) / 2.f +
                positionFactor * i,
            10 + bb.height / 2.f * SCALE
        );
        number->m_sprite->setScale(SCALE, SCALE);
    }
}

// Generiranje naključnega realnega števila
const float randomFloat(float min, float max) {
    return min + static_cast<float>(rand()) /
                     (static_cast<float>(RAND_MAX / (max - min)));
}

// makes pair of pipes, one up and one down
void makePipePair(
    EntityManager &entityManager, TextureManager &textureManager,
    sf::RenderWindow &window, unsigned int score
) {
    auto bottomPipe = entityManager.addEntity(EntityTag::pipe);
    auto topPipe = entityManager.addEntity(EntityTag::pipe);

    sf::Vector2f pipeShape(PIPEWIDTH * SCALE, PIPEHEIGHT * SCALE);
    sf::Vector2f collisionShape(pipeShape.x, pipeShape.y);
    bottomPipe->m_collisionShape = std::make_shared<CRectangle>(collisionShape);
    bottomPipe->m_collisionShape->shape.setOrigin(
        pipeShape.x / 2.f, pipeShape.y / 2.f
    );
    topPipe->m_collisionShape = std::make_shared<CRectangle>(pipeShape);
    topPipe->m_collisionShape->shape.setOrigin(
        pipeShape.x / 2.f, pipeShape.y / 2.f
    );

    float gapSize = randomFloat(MINGAP, MAXGAP);
    float gapMiddle = randomFloat(
        GAPPADDING + gapSize / 2.f,
        window.getSize().y - GAPPADDING - gapSize / 2.f - FLOORHEIGHT
    );

    float bottomPos = gapMiddle + gapSize / 2 + pipeShape.y / 2.f;
    float topPos = gapMiddle - gapSize / 2 - pipeShape.y / 2.f;

    bottomPipe->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(-PIPESPEED, 0),
        sf::Vector2f(window.getSize().x + pipeShape.x / 2.f, bottomPos), 0.f
    );
    topPipe->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(-PIPESPEED, 0),
        sf::Vector2f(window.getSize().x + pipeShape.x / 2.f, topPos), 180.f
    );

    bottomPipe->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tPipe));
    topPipe->m_sprite =
        std::make_shared<sf::Sprite>(textureManager.getTexture(tPipe));

    bottomPipe->m_sprite->setScale(-SCALE, SCALE);
    topPipe->m_sprite->setScale(SCALE, SCALE);

    sf::FloatRect bb = bottomPipe->m_sprite->getLocalBounds();
    bottomPipe->m_sprite->setOrigin(bb.width / 2.f, bb.height / 2.f);
    topPipe->m_sprite->setOrigin(bb.width / 2.f, bb.height / 2.f);

    topPipe->m_sprite->setRotation(180);

    // collision boxes which up score when touched
    auto scoreBox = entityManager.addEntity(EntityTag::scoreBox);
    scoreBox->m_cTransform = std::make_shared<CTransform>(
        sf::Vector2f(-PIPESPEED, 0),
        sf::Vector2f(window.getSize().x + pipeShape.x, gapMiddle), 0.f
    );
    scoreBox->m_collisionShape =
        std::make_shared<CRectangle>(sf::Vector2f(pipeShape.x / 2.f, gapSize));
    sf::FloatRect scorebb = scoreBox->m_collisionShape->shape.getLocalBounds();
    scoreBox->m_collisionShape->shape.setOrigin(
        scorebb.width / 2.f, scorebb.height / 2.f
    );
    scoreBox->m_collisionShape->shape.setPosition(scoreBox->m_cTransform->pos);
}

// handles gameovers
void GameScene::gameover() {
    if (m_score > m_best) {
        m_best = m_score;
    }

    sceneManager.remove("gameover");
    sceneManager.add(
        "gameover", std::make_shared<GameOverScene>(
                        sceneManager, entityManager, textureManager,
                        audioManager, inputManager, window, m_score, m_best
                    )
    );
    sceneManager.switchTo("gameover");
    audioManager.play(SoundTag::sHit);
}
