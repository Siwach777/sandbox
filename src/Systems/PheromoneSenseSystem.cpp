#include "Systems/PheromoneSenseSystem.hpp"
#include "ECS/World.hpp"
#include "World/PheromoneGrid.hpp"
#include "Config.hpp"
#include <cmath>

namespace Systems {
    void pheromoneSense(World& world, PheromoneGrid& pheromones, float tileSize, sf::Time dt) {
        float sensorDist = tileSize * 2.5f;
        float sensorAngle = 0.6f;
        float turnStrength = tileSize / 2.5f;

        float jitter = Random::getFloat(-0.3f, 0.3f);

        for (auto& [entity, heading] : world.headings) {
            if (!world.positions.count(entity)) { continue; }
            if (!world.ants.count(entity)) { continue; }

            PheromoneType trailType = PheromoneType::toFood;
            if (world.antBehaviors.count(entity) > 0) {
                if (world.antBehaviors[entity].state == AntState::Returning) {
                    trailType = PheromoneType::toHome;
                } else if (world.antBehaviors[entity].state == AntState::Idle) {
                    continue;
                }
            }

            auto& pos = world.positions[entity];
            float h = heading.angle;

            auto sample = [&] (float angleOffset) -> float {
                float sx = pos.x + std::cos(h + angleOffset) * sensorDist;
                float sy = pos.y + std::sin(h + angleOffset) * sensorDist;
                int gx = static_cast<int>(sx / tileSize);
                int gy = static_cast<int>(sy / tileSize);
                return pheromones.get(gx, gy, trailType);
            };

            float left = sample(-sensorAngle);
            float center = sample(0.f);
            float right = sample(sensorAngle);
            
            float seconds = dt.asSeconds();

            if (center > 0.01f || left > 0.01f || right > 0.01f) {
                if (center >= left && center >= right) { continue; }
                else if (left > right) {
                    heading.angle -= turnStrength * seconds;
                }
                else if (right > left) {
                    heading.angle += turnStrength * seconds;    
                }
            }
            else {
                heading.angle += jitter * seconds;
            }

            if (world.wanders.count(entity)) {
                float speed = world.wanders[entity].speed;
                if (world.velocities.count(entity)) {
                    world.velocities[entity].x = std::cos(heading.angle) * speed;
                    world.velocities[entity].y = std::sin(heading.angle) * speed;
                }
            }
        }
    }
}