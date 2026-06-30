#include "Systems/InteractionSystem.hpp"
#include "ECS/World.hpp"
#include <algorithm>

namespace Systems {
    void interaction(World &world) {
        for (Entity antEntity : world.ants) {
            if (!world.isAlive(antEntity) || world.positions.count(antEntity) == 0 || world.antBehaviors.count(antEntity) == 0) {
                continue;
            }

            auto& behavior = world.antBehaviors[antEntity];
            if (behavior.state != AntState::Foraging) { continue; }
            auto& antPos = world.positions[antEntity];

            for (auto& foodEntity : world.foods) {
                if (!world.isAlive(foodEntity) || world.foodAmounts.count(foodEntity) == 0 || world.positions.count(foodEntity) == 0) {
                    continue;
                }
                auto& foodPos = world.positions[foodEntity];
                auto& foodAmount = world.foodAmounts[foodEntity];

                float dx = antPos.x - foodPos.x;
                float dy = antPos.y - foodPos.y;
                float dist = dx*dx + dy*dy;

                float pickupRange = 10.f;
                if (dist < pickupRange * pickupRange) {
                    int pickup = std::min(10, foodAmount.amount);
                    foodAmount.amount -= pickup;

                    if (world.carryings.count(antEntity) == 0) {
                        world.carryings[antEntity] = {pickup};
                    } else {
                        world.carryings[antEntity].amount += pickup;
                    }

                    behavior.state = AntState::Returning;
                    behavior.stateTimer = 0.f;

                    if (foodAmount.amount <= 0) {
                        world.destroyEntity(foodEntity);
                    }
                    break;
                }
            }
        }
    }
}