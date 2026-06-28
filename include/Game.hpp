#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Camera.hpp"
#include "HUD.hpp"
#include "ECS/World.hpp"
#include "ECS/EntityFactory.hpp"
#include "Systems/MovementSystem.hpp"
#include "Systems/WanderSystem.hpp"
#include "Systems/RenderSystem.hpp"
#include "Random.hpp"

enum class ToolMode { TilePaint, FoodPlace };

class Game {
    public:
        Game();
        void run();

    private:
        void processEvents();
        void update(sf::Time dt);
        void render();

        Entity findEntity(sf::Vector2f worldPos);

        sf::RenderWindow m_window;
        sf::Vector2f m_mouseWorldPos;
        sf::Vector2i m_hoveredTile;

        Grid m_grid;
        Camera m_camera;
        HUD m_hud;
        World m_world;
        float m_fps = 0.f;

        TileType m_selectedTile = TileType::Dirt;
        bool m_isPainting = false;
        bool m_isErasing = false;
        Entity m_selectedEntity = INVALID_ENTITY;
        ToolMode m_toolMode = ToolMode::TilePaint;

        static constexpr float TICKS_PER_SECOND = 144.f;
        static constexpr sf::Time TIME_PER_TICK = sf::seconds(1.f / TICKS_PER_SECOND);
};

inline const char* toolMode(ToolMode mode) {
    switch(mode) {
        case ToolMode::TilePaint: return "Tile Painting";
        case ToolMode::FoodPlace: return "Food Placement";
        default                 : return "Unknown";
    }
}