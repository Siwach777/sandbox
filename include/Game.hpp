#pragma once
#include <SFML/Graphics.hpp>
#include "Grid.hpp"

class Game {
    public:
        Game();
        void run();

    private:
        void processEvents();
        void update(sf::Time dt);
        void render();

        sf::RenderWindow m_window;

        Grid m_grid;

        static constexpr float TICKS_PER_SECOND = 144.f;
        static constexpr sf::Time TIME_PER_TICK = sf::seconds(1.f / TICKS_PER_SECOND);
};