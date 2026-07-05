#include "Systems/RenderSystem.hpp"
#include "ECS/World.hpp"

namespace Systems {
    void render(World& world, sf::RenderWindow& window) {
        for (auto& [entity, pos] : world.positions) {
            if (world.renderables.count(entity) == 0) { continue; }

            auto& renderable = world.renderables[entity];

            sf::CircleShape shape(renderable.radius);
            shape.setFillColor(renderable.color);
            shape.setOrigin({renderable.radius, renderable.radius});
            shape.setPosition({pos.x, pos.y});
            window.draw(shape);
        }
    }
}