#pragma once

#include "../scenes/Scene.hpp"

class Scene;

class SceneManager {
  public:
    SceneManager();

    void run(const sf::Time dt);

    void add(std::string name, std::shared_ptr<Scene> scene);
    void switchTo(std::string name);
    void remove(std::string name);

    void turnOffGame();
    bool isTurnedOff() const;

  private:
    std::unordered_map<std::string, std::shared_ptr<Scene>> m_scenes;
    std::shared_ptr<Scene> m_currentScene;

    bool m_running = true;
};
