#include "EntityManager.hpp"
#include <algorithm>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

EntityManager::EntityManager() {}

// Doda entiteto in ji da enolični identifikator
std::shared_ptr<Entity> EntityManager::addEntity(const EntityTag tag) {
    auto e = std::shared_ptr<Entity>(
        new Entity(tag, m_totalEntites++));
    m_toAdd.emplace_back(e);
    return e;
}

// Vrne vektor entitet določenega tipa
EntityVec EntityManager::getEntities(const EntityTag tag) {
    return m_entityMap[tag];
}

// Vrne vse entitete
EntityVec EntityManager::getEntities() {
    return m_entities;
}

// Ali je entiteta mrtva
bool isDead(const std::shared_ptr<Entity> e) {
    return e->isDead();
}

// Odstrani vse entitete, ki niso več uporabljene
void EntityManager::destroyEntities(EntityVec &vec) {
    vec.erase(
        std::remove_if(vec.begin(), vec.end(), isDead),
        vec.end());
}

// Posodobi vse, ko je bilo že vse sprocesirano
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
