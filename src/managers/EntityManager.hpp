#pragma once

#include "Entity.hpp"
#include <unordered_map>

class EntityManager {
    typedef std::vector<std::shared_ptr<Entity>> EntityVec;

  public:
    EntityManager();
    // Funkcije za upravljanje z entitetami
    std::shared_ptr<Entity> addEntity(const EntityTag tag);
    // Različni glede na vektor, ki ga vrne
    std::vector<std::shared_ptr<Entity>> getEntities(); // Vrne vse
    std::vector<std::shared_ptr<Entity>>
    getEntities(const EntityTag tag); // Vrne le entitete določenega tipa

    // Posodobi vse entitete
    void update();

  private:
    // Tukaj se shranjujejo vse entitete
    std::unordered_map<EntityTag, EntityVec> m_entityMap;
    EntityVec m_entities;
    EntityVec m_toAdd;
    size_t m_totalEntites = 0; // Koliko entitete je bilo ustvarjenih

    // Odstranjevanje mrtvih entitet
    void destroyEntities(EntityVec &vec);
};
