#include "Entity.hpp"

Entity::Entity(EntityTag tag, int id)
    : m_tag{tag}, m_id{id}, m_alive{true} {}

void Entity::kill() { m_alive = false; }

bool Entity::isDead() { return !m_alive; }

EntityTag Entity::tag() { return m_tag; }

int Entity::id() { return m_id; }
