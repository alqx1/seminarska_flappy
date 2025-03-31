#pragma once

#include "Entity.hpp"
#include <unordered_map>

class EntityManager {
    typedef std::vector<std::shared_ptr<Entity>> EntityVec;

  public:
    EntityManager();
    std::shared_ptr<Entity> addEntity(const EntityTag tag);

    std::vector<std::shared_ptr<Entity>> getEntities();

    std::vector<std::shared_ptr<Entity>> getEntities(const EntityTag tag);

    void update();

  private:
    std::unordered_map<EntityTag, EntityVec> m_entityMap;
    EntityVec m_entities;
    EntityVec m_toAdd;
    size_t m_totalEntites = 0;

    void destroyEntities(EntityVec &vec);
};
