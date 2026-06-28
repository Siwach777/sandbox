#pragma once

#include <SFML/System/Time.hpp>
#include "ECS/World.hpp"

namespace Systems {
    void behavior(World& world, sf::Time dt);
}