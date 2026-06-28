#include "ECS/EntityFactory.hpp"

namespace EntityFactory {
    Entity createAnt(World& world, float x, float y) {
        Entity e = world.createEntity();
        world.positions[e] = {x, y};
        world.velocities[e] = {0.f, 0.f};
        world.renderables[e] = {3.f, sf::Color::Red};
        world.wanders[e] = {};
        world.ants.insert(e);
        return e;
    }

    Entity createFood(World& world, float x, float y) {
        Entity e = world.createEntity();
        world.positions[e] = {x, y};
        world.renderables[e] = {3.f, sf::Color::Green};
        world.foodAmounts[e] = {};
        world.foods.insert(e);
        return e;
    }
}
