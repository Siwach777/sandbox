#include "Systems/PheromoneSenseSystem.hpp"
#include "ECS/World.hpp"
#include "World/PheromoneGrid.hpp"
#include "Config.hpp"
#include "Random.hpp"
#include <cmath>

namespace Systems {
    void pheromoneSense(World& world, PheromoneGrid& pheromones, float tileSize, sf::Time dt) {
        
        float sensorDist   = tileSize * 9.f;     
        float sensorAngle  = 0.7f;              
        int   samplesPerRay = 8;                  
        float turnSpeed    = config.speed * 0.4f; 
        float wanderDrift  = 0.8f;               

        float seconds = dt.asSeconds();

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

            float antennaAngles[3] = { -sensorAngle, 0.f, sensorAngle }; // left, center, right
            float antennaScores[3] = { 0.f, 0.f, 0.f };

            for (int a = 0; a < 3; ++a) {
                float rayAngle = h + antennaAngles[a];
                float cosA = std::cos(rayAngle);
                float sinA = std::sin(rayAngle);

                for (int s = 1; s <= samplesPerRay; ++s) {
                    float dist = tileSize * 0.5f + (sensorDist - tileSize * 0.5f) * (static_cast<float>(s) / samplesPerRay);
                    float sx = pos.x + cosA * dist;
                    float sy = pos.y + sinA * dist;
                    int gx = static_cast<int>(sx / tileSize);
                    int gy = static_cast<int>(sy / tileSize);

                    float intensity = pheromones.get(gx, gy, trailType);
                    float weight = 1.f - 0.3f * (static_cast<float>(s) / samplesPerRay);
                    antennaScores[a] += intensity * weight;
                }
            }

            float leftScore   = antennaScores[0];
            float centerScore = antennaScores[1];
            float rightScore  = antennaScores[2];
            float maxScore    = std::max({leftScore, centerScore, rightScore});


            bool detected = (maxScore > 0.01f);
            if (world.antBehaviors.count(entity)) {
                world.antBehaviors[entity].followingPheromone = detected;
            }

            if (detected) {
                float totalScore = leftScore + centerScore + rightScore;
                float steer = (rightScore - leftScore) / totalScore;
                heading.angle += steer * turnSpeed * seconds;
            } else {
                heading.angle += Random::getFloat(-wanderDrift, wanderDrift) * seconds;
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