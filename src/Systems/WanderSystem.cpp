#include "Systems/WanderSystem.hpp"
#include "ECS/World.hpp"
#include "Random.hpp"
#include <cmath>

namespace Systems {
    void wander(World& world, sf::Time dt) {
        for (auto& [entity, wander] : world.wanders) {
            if (world.velocities.count(entity) == 0) { continue; }

            if (world.antBehaviors.count(entity) > 0) {
                if (world.antBehaviors[entity].state != AntState::Foraging) {
                    continue;
                }
            }

            auto& vel = world.velocities[entity];
            
            wander.timer -= dt.asSeconds();
            if (wander.timer <= 0.f) {
                float angle = Random::getFloat(0.f, 6.28318f);
                vel.x = std::cos(angle) * wander.speed;
                vel.y = std::sin(angle) * wander.speed;

                wander.timer = wander.changeInterval;
            }
        }
    }
}