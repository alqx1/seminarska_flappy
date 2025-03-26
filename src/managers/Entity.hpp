#pragma once

#include "../components/CInput.hpp"
#include "../components/CRectangle.hpp"
#include "../components/CTransform.hpp"
#include <SFML/Graphics/Sprite.hpp>
#include <memory>

enum EntityTag {
    bird,
    pipe,
    scoreBox,
    scoreNumber,
    background,
    button,
    base,
    text,
    menu
};

class Entity {
  public:
    std::shared_ptr<CTransform> m_cTransform;
    std::shared_ptr<CInput> m_cInput;
    std::shared_ptr<sf::Sprite> m_sprite;
    std::shared_ptr<CRectangle> m_collisionShape;

    void kill();
    bool isDead();
    EntityTag tag();
    int id();

  private:
    friend class EntityManager;

    Entity(EntityTag tag, int id);
    EntityTag m_tag;
    int m_id;
    bool m_alive;
};
