#include "Systems/BehaviorSystem.hpp"

namespace Systems {
    void behavior(World& world, sf::Time dt) {
        for (auto& [entity, behavior] : world.antBehaviors) {
            if (world.velocities.count(entity) == 0 || world.belongToNests.count(entity) == 0) { continue; }
            auto& vel = world.velocities[entity];
            auto& belongs = world.belongToNests[entity];
            behavior.stateTimer += dt.asSeconds();
            // Color State
            // if (world.renderables.count(entity) > 0) {
            //     auto&
            // }
            switch (behavior.state) {
                case AntState::Idle : 
                    vel.x = 0.f;
                    vel.y = 0.f;
                    if (behavior.stateTimer > 2.f) {
                        behavior.state = AntState::Foraging;
                        behavior.stateTimer = 0.f;
                    }
                    break;
                case AntState::Foraging :
                    if (behavior.stateTimer > 8.f) {
                        behavior.state = AntState::Returning;
                        behavior.stateTimer = 0.f;
                    }
                    break;
                case AntState::Returning :
                    if (world.isAlive(belongs.nest) && world.positions.count(belongs.nest) > 0) {
                        auto& nestPos = world.positions[belongs.nest];
                        auto& antPos = world.positions[entity];

                        float dx = nestPos.x - antPos.x;
                        float dy = nestPos.y - antPos.y;
                        float dist = std::sqrt(dx*dx + dy*dy);

                        if (dist < 10.f) {
                            behavior.state = AntState::Idle;
                            behavior.stateTimer = 0.f;
                        } else {
                            float speed = 50.f;
                            vel.x = (dx / dist) * speed;
                            vel.y = (dy / dist) * speed;
                        }
                    }
                    break;
            }
        }
    }
}