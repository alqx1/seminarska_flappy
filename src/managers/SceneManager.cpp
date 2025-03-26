#include "SceneManager.hpp"
#include <unordered_map>
#include <utility>

#include <SFML/Graphics.hpp>

SceneManager::SceneManager() {}

// run all selected scene's system functions
void SceneManager::run(const sf::Time dt) {
    if (m_currentScene) {
        m_currentScene->run(dt);
    }
}

void SceneManager::add(
    std::string name, std::shared_ptr<Scene> scene) {
    auto inserted =
        m_scenes.insert(std::make_pair(name, scene));
    inserted.first->second->init();
}

void SceneManager::switchTo(std::string id) {
    auto it = m_scenes.find(id);

    if (it != m_scenes.end()) {
        if (m_currentScene) {
            m_currentScene->onDeactivate();
        }

        m_currentScene = it->second;
        m_currentScene->onActivate();
    }
}

void SceneManager::remove(std::string id) {
    auto it = m_scenes.find(id);

    if (it != m_scenes.end()) {
        if (m_currentScene == it->second) {
            m_currentScene = nullptr;
        }

        m_scenes.erase(it);
    }
}

// get if key is pressed or not
bool SceneManager::getInputStatus(const int keyCode) const {
    auto it = m_inputs.find(keyCode);
    if (it != m_inputs.end()) {
        return it->second;
    }
    else {
        return false;
    }
}

void SceneManager::enableInput(const int keyCode) {
    m_inputs.insert_or_assign(
        m_inputs.begin(), keyCode, true);
}

void SceneManager::disableInput(const int keyCode) {
    m_inputs.insert_or_assign(
        m_inputs.begin(), keyCode, false);
}

// same as key
bool SceneManager::getMouseStatus(const int mouseButton) const {
    auto it = m_mouseButtons.find(mouseButton);
    if (it != m_mouseButtons.end()) {
        return it->second;
    }
    else {
        return false;
    }
}

void SceneManager::enableMouseButton(
    const int mouseButton,
    const sf::Vector2i mousePosition) {
    m_mouseButtons.insert_or_assign(
        m_mouseButtons.begin(), mouseButton, true);
    mousePos.x = mousePosition.x;
    mousePos.y = mousePosition.y;
}

void SceneManager::disableMouseButton(
    const int mouseButton) {
    m_mouseButtons.insert_or_assign(
        m_mouseButtons.begin(), mouseButton, false);
}

sf::Vector2f SceneManager::getMousePos() const {
    return mousePos;
}

void SceneManager::turnOffGame() {
    m_running = false;
}

// gets if game is turned off, used in GameEngine class
bool SceneManager::isTurnedOff() const {
    return !m_running;
}
