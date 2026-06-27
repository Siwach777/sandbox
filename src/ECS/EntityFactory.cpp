#include "ECS/EntityFactory.hpp"

namespace EntityFactory {
    Entity createAnt(World& world, float x, float y) {
        Entity e = world.createEntity();
        world.positions[e] = {x, y};
        world.velocities[e] = {0.f, 0.f};
        world.renderables[e] = {3.f, sf::Color(50, 40, 30)};
        world.wanders[e] = {};
        world.ants.insert(e);
        return e;
    }
}