#pragma once
#include <SFML/System/Time.hpp>

class World;

namespace Systems {
    void behavior(World& world, sf::Time dt);
}