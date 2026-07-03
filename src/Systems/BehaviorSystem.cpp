#include "Systems/BehaviorSystem.hpp"
#include "Random.hpp"
#include "ECS/World.hpp"
#include "Config.hpp"
#include <cmath>

namespace Systems {
    void behavior(World& world, sf::Time dt) {
        for (auto& [entity, behavior] : world.antBehaviors) {
            if (world.velocities.count(entity) == 0 || world.belongToNests.count(entity) == 0) { continue; }
            auto& vel = world.velocities[entity];
            auto& belongs = world.belongToNests[entity];
            behavior.stateTimer += dt.asSeconds();
            
            switch (behavior.state) {
                case AntState::Idle : 
                    vel.x = 0.f;
                    vel.y = 0.f;
                    if (behavior.stateTimer > 0.2f) {
                        behavior.state = AntState::Foraging;
                        behavior.stateTimer = 0.f;
                    }
                    break;
                case AntState::Foraging :
                    if (behavior.stateTimer > 60.f) {
                        if (world.headings.count(entity)) {
                            world.headings[entity].angle += 3.14159f /*+ Random::getFloat(-0.1f, 1.f)*/;
                        }
                        behavior.state = AntState::Returning;
                        behavior.stateTimer = 0.f;
                    }
                    break;
                case AntState::Returning :
                    if (world.isAlive(belongs.nest) && world.positions.count(belongs.nest) > 0) {
                        if (behavior.stateTimer > 20.f) {
                            if (world.headings.count(entity)) {
                                world.headings[entity].angle += Random::getFloat(-0.1f, 1.f);
                            }
                            behavior.stateTimer = 0.f;
                        }
                        auto& nestPos = world.positions[belongs.nest];
                        auto& antPos = world.positions[entity];

                        float dx = nestPos.x - antPos.x;
                        float dy = nestPos.y - antPos.y;
                        float dist = std::sqrt(dx*dx + dy*dy);

                        if (dist < 20.f) {
                            behavior.state = AntState::Idle;
                            behavior.stateTimer = 0.f;
                            if (world.headings.count(entity)) {
                                world.headings[entity].angle += 3.14159f /* + Random::getFloat(-0.1f, 1.f)*/;
                            }
                        } 
                    }
                    break;
            }
        }
    }
}