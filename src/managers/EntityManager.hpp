#pragma once

#include "Entity.hpp"
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager {
  public:
    EntityManager();
    std::shared_ptr<Entity> addEntity(const EntityTag tag);

    std::vector<std::shared_ptr<Entity>> getEntities();

    std::vector<std::shared_ptr<Entity>>
    getEntities(const EntityTag tag);

    void update();

  private:
    std::unordered_map<EntityTag, EntityVec> m_entityMap;
    EntityVec m_entities;
    EntityVec m_toAdd;
    size_t m_totalEntites;

    void destroyEntities(EntityVec &vec);
};
