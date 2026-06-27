#include "ECS/EntityFactory.hpp"

namespace EntityFactory {
    Entity createAnt(World& world, float x, float y) {
        Entity e = world.createEntity();
        world.positions[e] = {x, y};
        world.renderables[e] = {3.f, sf::Color(50, 40, 30)};
        world.ants.insert(e);
        return e;
    }
}