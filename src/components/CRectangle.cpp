#include "CRectangle.hpp"

CRectangle::CRectangle(const sf::Vector2f size)
    : shape(sf::RectangleShape(size)) {
    // template colors
    shape.setFillColor(sf::Color(0, 0, 0, 0));
    shape.setOutlineColor(sf::Color(255, 0, 0));
    shape.setOutlineThickness(-5);
}

// Preveri, če je točka znotraj lika
// Uporabimo pri koliziji miške z gumbom
bool CRectangle::isInside(const sf::Vector2f pos) {
    bool horizontal =
        pos.x > shape.getPosition().x &&
        pos.x < shape.getPosition().x + shape.getSize().x;
    bool vertical =
        pos.y > shape.getPosition().y &&
        pos.y < shape.getPosition().y + shape.getSize().y;
    return vertical && horizontal;
}
