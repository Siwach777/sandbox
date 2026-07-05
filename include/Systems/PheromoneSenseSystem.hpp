#pragma once
#include <SFML/System/Time.hpp>
#include "Random.hpp"

class World;
class PheromoneGrid;
class config;

namespace Systems {
    void pheromoneSense(World& world, PheromoneGrid& pheromones, float tileSize, sf::Time dt);
}