#pragma once

#include <SFML/System/Vector2.hpp>

struct CTransform {
    sf::Vector2f velocity;
    sf::Vector2f pos;
    float angle;

    CTransform() {}
    CTransform(const sf::Vector2f& velocity, const sf::Vector2f& pos, const float angle)
        : velocity{velocity}, pos{pos} {}
};
