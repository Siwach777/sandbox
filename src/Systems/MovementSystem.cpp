#include "Systems/MovementSystem.hpp"
#include "ECS/World.hpp"
#include "algorithm"

namespace Systems {
    void movement(World& world, sf::Time dt, float worldWidth, float worldHeight) {
        for (auto& [entity, pos] : world.positions) {
            if (world.velocities.count(entity) == 0) { continue; }
            
            auto& vel = world.velocities[entity];
            pos.x += vel.x * dt.asSeconds();
            pos.y += vel.y * dt.asSeconds();

            pos.x = std::clamp(pos.x, 0.f + 1.f, worldWidth - 1.f);
            pos.y = std::clamp(pos.y, 0.f + 1.f, worldHeight - 1.f);
        }
    }
} 