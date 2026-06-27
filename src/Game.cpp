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
            std::string title = "World Sim — " + std::to_string(frameCount) + " FPS";
            title += " | Tool: " + std::string(tileName(m_selectedTile));
            if (m_grid.inBounds(m_hoveredTile.x, m_hoveredTile.y))
            {
                title += " | Hover: " + std::string(tileName(
                    m_grid.getTile(m_hoveredTile.x, m_hoveredTile.y)));
                title += " (" + std::to_string(m_hoveredTile.x)
                    + "," + std::to_string(m_hoveredTile.y) + ")";
            }
            m_window.setTitle(title);

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

    sf::Vector2i mousePixel = sf::Mouse::getPosition(m_window);
    m_mouseWorldPos = m_window.mapPixelToCoords(mousePixel, m_camera.getView());
    m_hoveredTile = m_grid.worldToGrid(m_mouseWorldPos);

    if (m_isPainting && m_grid.inBounds(m_hoveredTile.x, m_hoveredTile.y)) {
        m_grid.setTile(m_hoveredTile.x, m_hoveredTile.y, m_selectedTile);        
    }
    if (m_isErasing &&m_grid.inBounds(m_hoveredTile.x, m_hoveredTile.y)) {
        m_grid.setTile(m_hoveredTile.x, m_hoveredTile.y, TileType::Dirt);        
    }

}

void Game::render() {
    m_window.clear(sf::Color(30, 30, 40));

    m_window.setView(m_camera.getView());
    m_grid.render(m_window);

    // Mouse Highlight
    if (m_grid.inBounds(m_hoveredTile.x, m_hoveredTile.y)) {
        float ts = m_grid.getTileSize();
        sf::RectangleShape highlight({ts - 1.f, ts - 1.f});
        highlight.setPosition({
            static_cast<float>(m_hoveredTile.x) * ts,
            static_cast<float>(m_hoveredTile.y) * ts
        });
        highlight.setFillColor(sf::Color(255, 255, 255, 60));
        highlight.setOutlineColor(sf::Color::White);
        highlight.setOutlineThickness(1.f);
        m_window.draw(highlight);
    }

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
            if (key->code == sf::Keyboard::Key::Num1) { m_selectedTile = TileType::Dirt; }
            if (key->code == sf::Keyboard::Key::Num2) { m_selectedTile = TileType::Stone; }
            if (key->code == sf::Keyboard::Key::Num3) { m_selectedTile = TileType::Sand; }
            if (key->code == sf::Keyboard::Key::Num4) { m_selectedTile = TileType::Water; }
            if (key->code == sf::Keyboard::Key::Num5) { m_selectedTile = TileType::Grass; }
            if (key->code == sf::Keyboard::Key::Num6) { m_selectedTile = TileType::Wall; }

        }
        // Mouse Scroll
        if (auto* scroll = event->getIf<sf::Event::MouseWheelScrolled>()) {
            float factor = (scroll->delta > 0) ? 0.9f : 1.1f;
            m_camera.zoom(factor);
        }
        // Mouse Button
        if (auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (pressed->button == sf::Mouse::Button::Left) {
                if (m_grid.inBounds(m_hoveredTile.x, m_hoveredTile.y)) {
                    m_window.setTitle("Clicked tile: (" +
                    std::to_string(m_hoveredTile.x) + ", " +
                    std::to_string(m_hoveredTile.y) + ")");
                    m_isPainting = true;
                }
            }
            else if (pressed->button == sf::Mouse::Button::Right) {
                m_isErasing = true;
            }
        }
        if (auto* released = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (released->button == sf::Mouse::Button::Left) { m_isPainting = false; }
            if (released->button == sf::Mouse::Button::Right) { m_isErasing = false; }
        }
    }
}