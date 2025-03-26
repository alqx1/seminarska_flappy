#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

struct CRectangle {
    sf::RectangleShape shape;

    CRectangle(const sf::Vector2f size);
    bool isInside(const sf::Vector2f pos);
};
