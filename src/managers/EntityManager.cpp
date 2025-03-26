#include "EntityManager.hpp"
#include <algorithm>

EntityManager::EntityManager() : m_totalEntites{0} {}

// adds entity and gives it a unique id
std::shared_ptr<Entity>
EntityManager::addEntity(const EntityTag tag) {
    auto e = std::shared_ptr<Entity>(
        new Entity(tag, m_totalEntites++));
    m_toAdd.emplace_back(e);
    return e;
}

// returns vector of certain entities
EntityVec EntityManager::getEntities(const EntityTag tag) {
    return m_entityMap[tag];
}

// returns all entities
EntityVec EntityManager::getEntities() {
    return m_entities;
}

// if entity has been killed
bool isDead(const std::shared_ptr<Entity> e) {
    return e->isDead();
}

void EntityManager::destroyEntities(EntityVec &vec) {
    vec.erase(
        std::remove_if(vec.begin(), vec.end(), isDead),
        vec.end());
}

// updates after everything else has been processed
void EntityManager::update() {
    for (auto &e : m_toAdd) {
        m_entityMap[e->tag()].emplace_back(e);
        m_entities.emplace_back(e);
    }
    m_toAdd.clear();

    destroyEntities(m_entities);
    for (auto &[tag, vec] : m_entityMap) {
        destroyEntities(vec);
    }
}
