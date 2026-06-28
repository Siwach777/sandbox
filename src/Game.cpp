#include "Game.hpp"
#include "Grid.hpp"
#include <iostream>
#include <string>

Game::Game() : m_window(sf::VideoMode({1280u, 720u}), "SIM")
             , m_grid(128, 72, 10.f)
             , m_camera({640.f, 360.f}, {640.f, 360.f}) {
    m_window.setFramerateLimit(static_cast<unsigned int>(TICKS_PER_SECOND));
    if (!m_hud.init("assets/fonts/JetBrainsMono-Bold.ttf")) {
        std::cout << "[ERROR] : Font didnt load" << std::endl;
    }

    // Entity

    Entity nestEntity = EntityFactory::createNest(m_world, 200.f, 200.f);
    auto& nestPos = m_world.positions[nestEntity];
    float offset = Random::getFloat(-30.f, +30.f);

    for (int i = 0; i < 50; ++i) {
        float ax = nestPos.x + offset;
        float ay = nestPos.y + offset;
        Entity e = EntityFactory::createAnt(m_world, ax, ay);
        m_world.belongToNests[e] = {nestEntity}; 
        auto& wander = m_world.wanders[e];
        wander.speed *= Random::getFloat(0.8f, 1.2f);
        wander.changeInterval *= Random::getFloat(0.5f, 1.5f);
    }

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

        if (fpsTimer >= sf::seconds(1.f)) {
            m_fps = static_cast<float>(frameCount); 
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

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) { m_toolMode = ToolMode::TilePaint; }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::F)) { m_toolMode = ToolMode::FoodPlace; }

    if (move.x != 0.f || move.y != 0.f) { 
        m_camera.move(move); 
    }

    m_camera.constrain({
        m_grid.getWidth() * m_grid.getTileSize(),
        m_grid.getHeight() * m_grid.getTileSize()
    });

    Systems::wander(m_world, dt);
    Systems::movement(m_world, dt, m_grid.getWidth() * m_grid.getTileSize(), m_grid.getHeight() * m_grid.getTileSize());

    // Mouse
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

    // ECS
    Systems::render(m_world, m_window);

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
    if (m_selectedEntity != INVALID_ENTITY && m_world.isAlive(m_selectedEntity) && m_world.positions.count(m_selectedEntity)) {
        auto& pos = m_world.positions[m_selectedEntity];

        sf::CircleShape ring(8.f);
        ring.setOrigin({8.f, 8.f});
        ring.setPosition({pos.x, pos.y});
        ring.setFillColor(sf::Color::Transparent);
        ring.setOutlineColor(sf::Color::Yellow);
        ring.setOutlineThickness(1.f);

        m_window.draw(ring);
    }

    // HUD
    std::string hoveredInfo;
    if (m_grid.inBounds(m_hoveredTile.x, m_hoveredTile.y)) {
        auto type = m_grid.getTile(m_hoveredTile.x, m_hoveredTile.y);
        hoveredInfo = std::string(tileName(type)) + " ("
                    + std::to_string(m_hoveredTile.x) + ", "
                    + std::to_string(m_hoveredTile.y) + ")";
    }
    std::string selectionInfo;
    if (m_selectedEntity != INVALID_ENTITY && m_world.isAlive(m_selectedEntity)) {
        if (m_world.ants.count(m_selectedEntity))
            selectionInfo += "Ant";
        if (m_world.foods.count(m_selectedEntity))
            selectionInfo += "Food" /*+ std::to_string((m_world.foodAmounts[m_selectedEntity]))*/;
        else 
            selectionInfo += "Entity";

        if (m_world.positions.count(m_selectedEntity)) {
            auto& pos = m_world.positions[m_selectedEntity];
            selectionInfo += " at (" + std::to_string(static_cast<int>(pos.x))
                           + ", " + std::to_string(static_cast<int>(pos.y)) + ")";
        }
    }
    std::string toolInfo = "Tool Mode : " + std::string(toolMode(m_toolMode));

    m_hud.render(m_window, m_fps, m_camera.getCenter(), m_camera.getZoom(), std::string(tileName(m_selectedTile)), hoveredInfo, selectionInfo, toolInfo);

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
            m_camera.constrain({
                m_grid.getWidth() * m_grid.getTileSize(),
                m_grid.getHeight() * m_grid.getTileSize()
            });
        }
        // Mouse Button
        if (auto* pressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (pressed->button == sf::Mouse::Button::Left) {
                if (m_toolMode == ToolMode::TilePaint) {
                    Entity found = findEntity(m_mouseWorldPos);
                    if (found != INVALID_ENTITY) {
                        m_selectedEntity = found;
                    }
                    else {
                        m_selectedEntity = INVALID_ENTITY;
                        m_isPainting = true;
                    }
                } else if (m_toolMode == ToolMode::FoodPlace) {
                    EntityFactory::createFood(m_world, m_mouseWorldPos.x, m_mouseWorldPos.y);
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

Entity Game::findEntity(sf::Vector2f worldPos) {
    float closestDist = 15.f;
    Entity closest = INVALID_ENTITY;

    for (auto& [entity, pos] : m_world.positions) {
        float dx = pos.x - worldPos.x;
        float dy = pos.y - worldPos.y;
        float dist = std::sqrt(dx*dx + dy*dy);
        if (dist < closestDist) {
            closestDist = dist;
            closest = entity;
        }
    }
    return closest;
}