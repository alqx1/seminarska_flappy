#include "InputManager.hpp"

InputManager::InputManager() {
}

bool InputManager::getInputStatus(const int keyCode) const {
    auto it = m_inputs.find(keyCode);
    if (it != m_inputs.end()) {
        return it->second;
    }
    else {
        return false;
    }
}

void InputManager::enableInput(const int keyCode) {
    m_inputs.insert_or_assign(m_inputs.begin(), keyCode, true);
}

void InputManager::disableInput(const int keyCode) {
    m_inputs.insert_or_assign(m_inputs.begin(), keyCode, false);
}

bool InputManager::getMouseStatus(const int mouseButton) const {
    auto it = m_mouseButtons.find(mouseButton);
    if (it != m_mouseButtons.end()) {
        return it->second;
    }
    else {
        return false;
    }
}

void InputManager::enableMouseButton(const int mouseButton) {
    m_mouseButtons.insert_or_assign(m_mouseButtons.begin(), mouseButton, true);
}

void InputManager::disableMouseButton(const int mouseButton) {
    m_mouseButtons.insert_or_assign(m_mouseButtons.begin(), mouseButton, false);
}

sf::Vector2f InputManager::getMousePos() const {
    return m_mousePos;
}

void InputManager::setMousePos(const sf::Vector2i mousePosition) {
    m_mousePos.x = mousePosition.x;
    m_mousePos.y = mousePosition.y;
}
