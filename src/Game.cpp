#include "Game.hpp"
#include "Grid.hpp"
#include "ECS/EntityFactory.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/WanderSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Systems/BehaviorSystem.hpp"
#include "Systems/InteractionSystem.hpp"
#include "Systems/PheromoneRenderSystem.hpp"
#include "UI/DebugPanel.hpp"
#include "Config.hpp"
#include "Random.hpp"
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <cmath>
#include <optional>
#include <iostream>
#include <string>
#include <imgui.h>
#include <imgui-SFML.h>

Game::Game() : m_window(sf::VideoMode(sf::Vector2u(config.WIN_WIDTH, config.WIN_HEIGHT)), "SIM")
             , m_grid(config.GRID_WIDTH, config.GRID_HEIGHT, config.TILE_SIZE)
             , m_pheromones(config.GRID_WIDTH, config.GRID_HEIGHT)
             , m_camera(sf::Vector2f(config.WIN_WIDTH/2.f, config.WIN_HEIGHT/2.f), sf::Vector2f({config.WIN_WIDTH/2.f, config.WIN_HEIGHT/2.f})) {
    m_window.setFramerateLimit(static_cast<unsigned int>(TICKS_PER_SECOND));
    if (!m_hud.init("assets/fonts/JetBrainsMono-Bold.ttf")) {
        std::cout << "[ERROR] : Font didnt load" << std::endl;
    }
    if (!ImGui::SFML::Init(m_window)) {
        std::cout << "[ERROR] : ImGui didnt load" << std::endl;
    }

    // Entity

    Entity nestEntity = EntityFactory::createNest(m_world, 640.f, 300.f);
    auto& nestPos = m_world.positions[nestEntity];

    for (int i = 0; i < 1; ++i) {
        float ax = nestPos.x + Random::getFloat(-30.f, +30.f);
        float ay = nestPos.y + Random::getFloat(-30.f, +30.f);
        Entity e = EntityFactory::createAnt(m_world, ax, ay);
        m_world.belongToNests[e] = {nestEntity}; 
        auto& wander = m_world.wanders[e];
        wander.speed *= Random::getFloat(-1.2f, 1.2f);
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
        if (elapsed > sf::seconds(0.1f)) {
            elapsed = sf::seconds(0.1f);
        }
        accumulator += elapsed * config.simSpeed;

        processEvents();

        while (accumulator >= TIME_PER_TICK) {
            update(TIME_PER_TICK);
            accumulator -= TIME_PER_TICK;
        }
        ImGui::SFML::Update(m_window, elapsed);
        DebugPanel::showStats(m_fps, static_cast<int>(m_world.ants.size()), static_cast<int>(m_world.foods.size()), m_camera.getCenter(), m_camera.getZoom(), m_pheromones);
        DebugPanel::showEntityList(m_world, m_selectedEntity);
        DebugPanel::showInspector(m_world, m_selectedEntity);
        DebugPanel::showControls();

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
    ImGui::SFML::Shutdown();
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

    // Handle Spawning Requests from ImGui
    if (config.spawnAntsRequested) {
        Entity nestEntity = INVALID_ENTITY;
        if (!m_world.nests.empty()) {
            nestEntity = *m_world.nests.begin();
        }
        if (nestEntity != INVALID_ENTITY) {
            auto& nestPos = m_world.positions[nestEntity];
            for (int i = 0; i < config.spawnCount; ++i) {
                float ax = nestPos.x + Random::getFloat(-30.f, +30.f);
                float ay = nestPos.y + Random::getFloat(-30.f, +30.f);
                Entity e = EntityFactory::createAnt(m_world, ax, ay);
                m_world.belongToNests[e] = {nestEntity};
                auto& wander = m_world.wanders[e];
                wander.speed = config.speed * Random::getFloat(0.8f, 1.2f);
                wander.changeInterval = config.wanderInterval * Random::getFloat(0.8f, 1.2f);
            }
        }
        config.spawnAntsRequested = false;
    }

    if (config.spawnFoodRequested) {
        sf::Vector2f center = m_camera.getCenter();
        for (int i = 0; i < config.spawnCount; ++i) {
            float fx = center.x + Random::getFloat(-100.f, +100.f);
            float fy = center.y + Random::getFloat(-100.f, +100.f);
            EntityFactory::createFood(m_world, fx, fy);
        }
        config.spawnFoodRequested = false;
    }

    // Propagate config changes to existing ants
    for (auto& [entity, wander] : m_world.wanders) {
        wander.speed = config.speed;
        wander.changeInterval = config.wanderInterval;
    }

    if (!config.paused) {
        for (Entity entity : m_world.ants) {
            if (m_world.positions.count(entity) == 0) { continue; }
            auto& pos = m_world.positions[entity];

            int gx = static_cast<int>(pos.x / m_grid.getTileSize());
            int gy = static_cast<int>(pos.y / m_grid.getTileSize());
            m_pheromones.deposit(gx, gy, config.pheromoneDepositAmount);
        }
        m_pheromones.evaporate(config.pheromoneEvapRate);
        m_pheromones.diffuse(config.pheromoneDiffusionRate);
        Systems::interaction(m_world);
        Systems::behavior(m_world, dt);
        Systems::wander(m_world, dt);
        Systems::movement(m_world, dt, m_grid.getWidth() * m_grid.getTileSize(), m_grid.getHeight() * m_grid.getTileSize());
    }

    

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
    if (config.showPheromones) {
    Systems::renderPheromones(m_pheromones, m_window, m_grid.getTileSize());
    }
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

        float radius = 4.f;
        if (m_world.renderables[m_selectedEntity].radius) {
            radius = m_world.renderables[m_selectedEntity].radius;
        }
        float ringRadius = radius + 2.f;
        sf::CircleShape ring(ringRadius);
        ring.setOrigin({ringRadius, ringRadius});
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
        if (m_world.ants.count(m_selectedEntity)) {
            selectionInfo += "Ant";
            auto& behavior = m_world.antBehaviors[m_selectedEntity];
            switch (behavior.state) {
                case AntState::Idle:      selectionInfo += " (Idle) ";      break;
                case AntState::Foraging:  selectionInfo += " (Foraging) ";  break;
                case AntState::Returning: selectionInfo += " (Returning) "; break;
            }
        }
        if (m_world.foods.count(m_selectedEntity))
            selectionInfo += "Food (" + std::to_string((m_world.foodAmounts[m_selectedEntity].amount)) + ") ";
        else 
            selectionInfo += "Entity";

        if (m_world.positions.count(m_selectedEntity)) {
            auto& pos = m_world.positions[m_selectedEntity];
            selectionInfo += " at (" + std::to_string(static_cast<int>(pos.x))
                           + ", " + std::to_string(static_cast<int>(pos.y)) + ")";
        }
    }
    std::string toolInfo = "Tool Mode : " + std::string(toolMode(m_toolMode));

    HUDData hudData {
        .fps = m_fps,
        .cameraPos = m_camera.getCenter(),
        .zoom = m_camera.getZoom(),
        .selectedTileName = tileName(m_selectedTile),
        .hoveredInfo = hoveredInfo,
        .selectionInfo = selectionInfo,
        .toolInfo = toolInfo/*,
        .stateName = stateInfo*/
    };

    m_hud.render(m_window, hudData);

    ImGui::SFML::Render(m_window);

    m_window.display();
}

void Game::processEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        }

        // ImGui
        ImGui::SFML::ProcessEvent(m_window, *event);

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

        if (ImGui::GetIO().WantCaptureMouse)
            continue;

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