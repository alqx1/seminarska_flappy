#pragma once

#include <SFML/Graphics.hpp>

// Struktura za kolizijski lik
struct CRectangle {
    sf::RectangleShape shape;

    CRectangle(const sf::Vector2f size);
    bool isInside(const sf::Vector2f pos);
};
