#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.hpp"
#include "Camera.hpp"

class Game {
    public:
        Game();
        void run();

    private:
        void processEvents();
        void update(sf::Time dt);
        void render();

        sf::RenderWindow m_window;
        sf::Vector2f m_mouseWorldPos;
        sf::Vector2i m_hoveredTile;

        Grid m_grid;
        Camera m_camera;

        TileType m_selectedTile = TileType::Dirt;
        bool m_isPainting = false;
        bool m_isErasing = false;

        static constexpr float TICKS_PER_SECOND = 144.f;
        static constexpr sf::Time TIME_PER_TICK = sf::seconds(1.f / TICKS_PER_SECOND);
};