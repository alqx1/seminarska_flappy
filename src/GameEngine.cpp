#include "GameEngine.hpp"
#include "Constants.hpp"
#include "scenes/MainMenuScene.hpp"

GameEngine::GameEngine() {
    init();
}

void GameEngine::init() {
    // Za generiranje naključnih števil
    srand(time(NULL));

    // Pripravljanje okna
    sf::VideoMode desktop = sf::VideoMode::getDesktopMode();
    m_window.create(
        sf::VideoMode(500, 700), "Flappy Bird",
        sf::Style::Titlebar | sf::Style::Close
    );
    m_window.setFramerateLimit(FPS);
    m_window.setKeyRepeatEnabled(false);

    // Dodajanje začetne scene v urejevalnik scen
    m_sceneManager.add(
        "mainmenu", std::make_shared<MainMenuScene>(
                        m_sceneManager, m_entityManager, m_textureManager,
                        m_audioManager, m_inputManager, m_window
                    )
    );
    m_sceneManager.switchTo("mainmenu");
}

void GameEngine::run() {
    // Shranjuje časovni zamik od med prejšno in to sliko
    sf::Clock deltaClock;
    while (m_running) {
        sf::Time dt = deltaClock.restart();

        processEvents();
        // Posodobi morebitne spremembe na vseh entitetah
        m_entityManager.update();

        // Kliče funkcijo za delovanje določene scene
        m_sceneManager.run(dt);
        m_window.display();
        m_window.clear(sf::Color::Black);
    }

    // Zapri okno, ko je konec delovanja
    m_window.close();
}

// Skrbi za vse dogodke za okno
// Skrbi tudi zapiranje in razne vnose
void GameEngine::processEvents() {
    sf::Event event;
    // Preide vse dogodke, ki so se zgodili
    // in naredi kaj, če je ta dogodek potreben
    while (m_window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            turnOff();
        }

        if (event.type == sf::Event::KeyPressed) {
            m_inputManager.enableInput(event.key.code);
        }

        if (event.type == sf::Event::KeyReleased) {
            m_inputManager.disableInput(event.key.code);
        }

        if (event.type == sf::Event::MouseButtonPressed) {
            m_inputManager.enableMouseButton(event.mouseButton.button);
        }

        if (event.type == sf::Event::MouseButtonReleased) {
            m_inputManager.disableMouseButton(event.mouseButton.button);
        }
    }
    m_inputManager.setMousePos(sf::Mouse::getPosition(m_window));

    // Če se je delovanje končalo, ustavi program
    if (m_sceneManager.isTurnedOff()) {
        turnOff();
    }
}

void GameEngine::turnOff() {
    m_running = false;
}
