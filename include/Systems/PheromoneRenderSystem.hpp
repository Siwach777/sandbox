#pragma once
#include <SFML/Graphics.hpp>

class PheromoneGrid;

namespace Systems {
    void renderPheromones(PheromoneGrid& grid, sf::RenderWindow& window, float tileSize);
}