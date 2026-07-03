#include "Systems/PheromoneSenseSystem.hpp"
#include "ECS/World.hpp"
#include "World/PheromoneGrid.hpp"
#include "Config.hpp"
#include <cmath>

namespace Systems {
    void pheromoneSense(World& world, PheromoneGrid& pheromones, float tileSize, sf::Time dt) {
        float sensorDist = tileSize * 8.f;
        float sensorAngle = 0.7f;
        float turnStrength = config.speed * 0.5f;

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

            float bestIntensity = 0.f;
            float bestDirection = 0.f;
            uint8_t sampleCount = 32;

            for (int i = 0; i < sampleCount; ++i) {
                float randomAngle = Random::getFloat(-sensorAngle, sensorAngle);
                float randomDist = Random::getFloat(tileSize, sensorDist);

                float sx = pos.x + std::cos(h + randomAngle) * randomDist;
                float sy = pos.y + std::sin(h + randomAngle) * randomDist;
                int gx = static_cast<int>(sx / tileSize);
                int gy = static_cast<int>(sy / tileSize);

                float intenstiy = pheromones.get(gx, gy, trailType);
                if (intenstiy > bestIntensity) {
                    bestIntensity = intenstiy;
                    bestDirection = randomAngle;
                }
            }

            float seconds = dt.asSeconds();
            if (bestIntensity > 0.01f) {
                if (bestDirection > 0.01f) {
                    heading.angle += bestDirection * turnStrength * seconds;
                }
            } else {
                float jitter = Random::getFloat(-0.175f, 0.175f);
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