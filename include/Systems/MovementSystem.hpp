#pragma once
#include <SFML/System/Time.hpp>

class World;

namespace Systems {
    void movement(World& world, sf::Time dt, float worldWidth, float worldHeight);
}