#include "Systems/PheromoneSenseSystem.hpp"
#include "ECS/World.hpp"
#include "World/PheromoneGrid.hpp"
#include "Config.hpp"
#include <cmath>

namespace Systems {
    void pheromoneSense(World& world, PheromoneGrid& pheromones, float tileSize, sf::Time dt) {
        float sensorDist = tileSize * 6.f;
        float sensorAngle = 0.75f;
        float turnStrength = tileSize / 3.f;

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
                int centerGx = static_cast<int>(sx / tileSize);
                int centerGy = static_cast<int>(sy / tileSize);
                float sum = 0.f;
                int count = 0;
                for (int dy = -1; dy <= 1; ++dy) {
                    for (int dx = -1; dx <= 1; ++dx) {
                        int gx = centerGx + dx;
                        int gy = centerGy + dy;
                        sum += pheromones.get(gx, gy, trailType);
                        count++;
                    }
                }
                return sum / static_cast<float>(count);
            };

            float left = sample(-sensorAngle);
            float center = sample(0.f);
            float right = sample(sensorAngle);
            
            float seconds = dt.asSeconds();

            if (center > 0.01f || left > 0.01f || right > 0.01f) {
                if (center >= left && center >= right) {
                    heading.angle += Random::getFloat(-0.001f, 0.001f) * seconds;
                } else {
                    float difference = (right - left) / (left + right + 0.001f);
                    heading.angle += difference * turnStrength * seconds;
                } 
            }
            else {
                float jitter = Random::getFloat(-0.2f, 0.2f);
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