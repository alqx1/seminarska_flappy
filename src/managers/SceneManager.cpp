#include "SceneManager.hpp"

SceneManager::SceneManager() {
}

// run all selected scene's system functions
void SceneManager::run(const sf::Time dt) {
    if (m_currentScene) {
        m_currentScene->run(dt);
    }
}

void SceneManager::add(std::string name, std::shared_ptr<Scene> scene) {
    auto inserted = m_scenes.insert(std::make_pair(name, scene));
    inserted.first->second->init();
    m_sceneCounter++;
}

void SceneManager::switchTo(std::string name) {
    auto it = m_scenes.find(name);

    if (it != m_scenes.end()) {
        if (m_currentScene) {
            m_currentScene->onDeactivate();
        }

        m_currentScene = it->second;
        m_currentScene->onActivate();
    }
}

void SceneManager::remove(std::string name) {
    auto it = m_scenes.find(name);

    if (it != m_scenes.end()) {
        if (m_currentScene == it->second) {
            m_currentScene = nullptr;
        }

        m_scenes.erase(it);
    }
}

void SceneManager::turnOffGame() {
    m_running = false;
}

// gets if game is turned off, used in GameEngine class
bool SceneManager::isTurnedOff() const {
    return !m_running;
}
