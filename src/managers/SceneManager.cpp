#include "SceneManager.hpp"

SceneManager::SceneManager() {
}

void SceneManager::run(const sf::Time dt) {
    // Če je scena določeno, zaženemo njeno delovanje
    if (m_currentScene) {
        m_currentScene->run(dt);
    }
}

// Dodamo sceno in jo v slovarju shranimo pod določenim imenom
void SceneManager::add(std::string name, std::shared_ptr<Scene> scene) {
    auto inserted = m_scenes.insert_or_assign(name, scene);
    inserted.first->second->init();
}

// Če je scena v slovarju, jo določimo
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

// Če je scena v slovarju, jo odstranimo
// V primeru, da je bila določeno, nastavimo
// trenutno sceno na nič
void SceneManager::remove(std::string name) {
    auto it = m_scenes.find(name);

    if (it != m_scenes.end()) {
        if (m_currentScene == it->second) {
            m_currentScene = nullptr;
        }

        m_scenes.erase(it);
    }
}

// Ugasnemo igro
void SceneManager::turnOffGame() {
    m_running = false;
}

bool SceneManager::isTurnedOff() const {
    return !m_running;
}
