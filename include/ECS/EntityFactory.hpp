#pragma once
#include "World.hpp"

namespace EntityFactory {
    Entity createAnt(World& world, float x, float y);
    Entity createFood (World& world, float x, float y);
}
