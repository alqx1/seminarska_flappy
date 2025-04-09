#include "InputManager.hpp"

InputManager::InputManager() {
}

// Če stanje tipke obstaja, ga vrnemo
bool InputManager::getInputStatus(const int keyCode) const {
    auto it = m_inputs.find(keyCode);
    if (it != m_inputs.end()) {
        return it->second;
    }
    else {
        return false;
    }
}

// Prižgemo tipko
void InputManager::enableInput(const int keyCode) {
    m_inputs.insert_or_assign(m_inputs.begin(), keyCode, true);
}

// Ugasnemo tipko
void InputManager::disableInput(const int keyCode) {
    m_inputs.insert_or_assign(m_inputs.begin(), keyCode, false);
}

// Če obstaja stanje miškinega gumba, ga vrnemo
bool InputManager::getMouseStatus(const int mouseButton) const {
    auto it = m_mouseButtons.find(mouseButton);
    if (it != m_mouseButtons.end()) {
        return it->second;
    }
    else {
        return false;
    }
}

// Prižgemo gumb
void InputManager::enableMouseButton(const int mouseButton) {
    m_mouseButtons.insert_or_assign(m_mouseButtons.begin(), mouseButton, true);
}

// Ugasnemo gumb
void InputManager::disableMouseButton(const int mouseButton) {
    m_mouseButtons.insert_or_assign(m_mouseButtons.begin(), mouseButton, false);
}

// Vrnemo pozicijo miške
sf::Vector2f InputManager::getMousePos() const {
    return m_mousePos;
}

// Nastavimo pozicijo miške
void InputManager::setMousePos(const sf::Vector2i mousePosition) {
    m_mousePos.x = mousePosition.x;
    m_mousePos.y = mousePosition.y;
}
