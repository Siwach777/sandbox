#include "Game.hpp"
#include "Grid.hpp"

Game::Game() : m_window(sf::VideoMode({1280u, 720u}), "SIM"), m_grid(100, 100, 16.f) {
    m_window.setFramerateLimit(static_cast<unsigned int>(TICKS_PER_SECOND));
}

void Game::run() {
    sf::Clock clock;
    sf::Time accumulator = sf::Time::Zero;

    uint16_t frameCount = 0;
    sf::Time fpsTimer = sf::Time::Zero;

    while (m_window.isOpen()) {
        sf::Time elapsed = clock.restart();
        accumulator += elapsed;

        processEvents();

        while (accumulator >= TIME_PER_TICK) {
            update(TIME_PER_TICK);
            accumulator -= TIME_PER_TICK;
        }

        render();

        ++frameCount;
        fpsTimer += elapsed;

        if (fpsTimer >= sf::seconds(1.f))
        {
            m_window.setTitle(
                "Sim - " + std::to_string(frameCount) + " FPS"
            );

            frameCount = 0;
            fpsTimer -= sf::seconds(1.f);
        }
    }
}

void Game::update(sf::Time dt) {

}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 40));

    m_grid.render(m_window);

    m_window.display();
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
        if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape) {
                m_window.close();
            }
        }
    }
}