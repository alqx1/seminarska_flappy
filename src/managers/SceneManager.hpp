#pragma once

#include "../scenes/Scene.hpp"

class Scene;

class SceneManager {
  public:
    SceneManager();

    // Zažene delovanje za določeno sceno
    void run(const sf::Time dt);

    // Funkcije za dodajanje, spreminjanje in odstranjevanje scene
    void add(std::string name, std::shared_ptr<Scene> scene);
    void switchTo(std::string name);
    void remove(std::string name);

    // Za ugašanje igre
    void turnOffGame();
    bool isTurnedOff() const;

  private:
    // Tukaj shranjujemo scene in trenutno sceno
    std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene> m_currentScene;

    bool m_running = true;
};
