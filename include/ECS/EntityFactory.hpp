#pragma once
#include "Entity.hpp"

class World;

namespace EntityFactory {
    Entity createAnt(World& world, float x, float y);
    Entity createFood(World& world, float x, float y);
    Entity createNest(World& world, float x, float y);
}
