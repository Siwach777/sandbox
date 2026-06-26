#include "Game.hpp"
#include "Grid.hpp"

Game::Game() : m_window(sf::VideoMode({1280u, 720u}), "SIM")
             , m_grid(100, 100, 16.f)
             , m_camera({640.f, 360.f}, {640.f, 360.f}) {
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
        auto center = m_camera.getCenter();

        if (fpsTimer >= sf::seconds(1.f))
        {
            m_window.setTitle("World Sim — " + std::to_string(frameCount) + " FPS"
                + " | Pos: " + std::to_string(static_cast<int>(center.x))
                + "," + std::to_string(static_cast<int>(center.y))
                + " | Zoom: " + std::to_string(m_camera.getZoom()).substr(0, 4));

            frameCount = 0;
            fpsTimer -= sf::seconds(1.f);
        }
    }
}

void Game::update(sf::Time dt) {

    float speed = 200.f * m_camera.getZoom() * dt.asSeconds();
    sf::Vector2f move{0.f, 0.f};
    
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W)) { move.y -= speed; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) { move.x -= speed; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) { move.y += speed; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) { move.x += speed; }

    if (move.x != 0.f || move.y != 0.f) { 
        m_camera.move(move); 
    }

}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 40));

    m_window.setView(m_camera.getView());
    m_grid.render(m_window);

    m_window.display();
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }
        // Keyboard
        if (auto* key = event->getIf<sf::Event::KeyPressed>()) {
            if (key->code == sf::Keyboard::Key::Escape) {
                m_window.close();
            }
        }
        // Mouse
        if (auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
            float factor = (scroll->delta > 0) ? 0.9f : 1.1f;
            m_camera.zoom(factor);
        }
    }
}