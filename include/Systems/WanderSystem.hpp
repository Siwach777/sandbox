#pragma once
#include <SFML/System/Time.hpp>

class World;

namespace Systems {
    void wander(World& world, sf::Time dt);
}