#pragma once

#include "../scenes/Scene.hpp"
#include <SFML/System.hpp>
#include <memory>
#include <string>
#include <unordered_map>

class SceneManager {
  public:
    SceneManager();

    void run(const sf::Time dt);

    void
    add(std::string name, std::shared_ptr<Scene> scene);
    void switchTo(std::string);
    void remove(std::string);

    bool getInputStatus(const int keyCode) const;
    void enableInput(const int keyCode);
    void disableInput(const int keyCode);

    bool getMouseStatus(const int mouseButton) const;
    void enableMouseButton(
        const int mouseButton,
        const sf::Vector2i mousePosition);
    void disableMouseButton(
        const int mouseButton);
    sf::Vector2f getMousePos() const;

    void turnOffGame();
    bool isTurnedOff() const;

  private:
    std::unordered_map<unsigned int, bool> m_inputs;
    std::unordered_map<unsigned int, bool> m_mouseButtons;
    std::unordered_map<std::string, std::shared_ptr<Scene>>
        m_scenes;
    std::shared_ptr<Scene> m_currentScene;

    unsigned int sceneCounter;
    sf::Vector2f mousePos;

    bool m_running = true;
};
