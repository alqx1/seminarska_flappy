#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

class InputManager {
  public:
    InputManager();

    bool getInputStatus(const int keyCode) const;
    void enableInput(const int keyCode);
    void disableInput(const int keyCode);

    bool getMouseStatus(const int mouseButton) const;
    void
    enableMouseButton(const int mouseButton);
    void disableMouseButton(const int mouseButton);

    sf::Vector2f getMousePos() const;
    void setMousePos(const sf::Vector2i mousePosition);

  private:
    std::unordered_map<unsigned int, bool> m_mouseButtons;
    std::unordered_map<unsigned int, bool> m_inputs;

    sf::Vector2f m_mousePos;
};
