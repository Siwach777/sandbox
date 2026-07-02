#include "UI/DebugPanel.hpp"
#include "ECS/World.hpp"
#include "Config.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

void DebugPanel::showStats(float fps, int antCount, int foodCount, sf::Vector2f cameraPos, float zoom, PheromoneGrid m_pheromones) {
    ImGui::Begin("Simulation");

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Separator();
    ImGui::Text("Ants: %d", antCount);
    ImGui::Text("Food sources: %d", foodCount);
    ImGui::Separator();
    ImGui::Text("Pheromone gridL %dx%d", m_pheromones.getWidth(), m_pheromones.getHeight());
    ImGui::Text("Max pheromone: %.3f", m_pheromones.getMaxValue());
    ImGui::Separator();
    ImGui::Text("Camera: (%0.f, %0.f)", cameraPos.x, cameraPos.y);
    ImGui::Text("Zoom: %.2f", zoom);

    ImGui::End();
}

void DebugPanel::showEntityList(World& world, Entity& selectedEntity) {
    ImGui::Begin("Entities");

    int index = 0;
    for (Entity entity : world.ants) {
        std::string label = "Ants #" + std::to_string(index);

        bool isSelected = (entity == selectedEntity);
        if (ImGui::Selectable(label.c_str(), isSelected)) {
            selectedEntity = entity;
        }
        ++index;
    }
    ImGui::End();
}

void DebugPanel::showInspector(World& world, Entity& entity) {
    ImGui::Begin("Inspector");

    if (entity == INVALID_ENTITY || !world.isAlive(entity)) {
        ImGui::Text("Nothing selected");
        ImGui::End();
        return;
    }
    if (world.velocities.count(entity)) {
        auto& behavior = world.antBehaviors[entity];
        const char* stateNames[] = {"Idle", "Foraging", "Returning"};
        ImGui::Text("State: %s", stateNames[static_cast<int>(behavior.state)]);
        ImGui::Text("State timer: %.1f", behavior.stateTimer);
    }
    if (world.carryings.count(entity)) {
        auto& carrying = world.carryings[entity];
        ImGui::Text("Carrying: %d food", carrying.amount);
    }

    ImGui::End();
}

void DebugPanel::showControls() {
    ImGui::Begin("Controls");

    ImGui::Checkbox("Pause Simulation", &config.paused);
    ImGui::Separator();

    ImGui::Checkbox("Show Pheromones", &config.showPheromones);
    ImGui::Separator();

    ImGui::SliderFloat("Simulation Speed", &config.simSpeed, 0.1f, 50.f);
    ImGui::Separator();

    ImGui::SliderFloat("Diffusion rate", &config.pheromoneDiffusionRate, 0.001f, 1.f);
    ImGui::Separator();

    // ImGui::SliderFloat("Evaporation rate", &config.pheromoneEvapRate, 0.1f, 2.f);
    // ImGui::Separator();

    ImGui::Text("Movement");
    ImGui::SliderFloat("Base Speed", &config.speed, 10.f, 100.f);
    ImGui::SliderFloat("Wander Interval", &config.changeInterval, 0.1f, 10.0f);

    ImGui::Separator();
    ImGui::Text("Interaction");
    ImGui::SliderFloat("Nest Distance threshold", &config.foodDist, 1.f, 50.f);
    ImGui::SliderFloat("Food Pickup Range", &config.foodPickupRange, 1.f, 50.f);
    ImGui::SliderInt("Food Pickup Amount", &config.foodPickupAmount, 1, 100);

    ImGui::Separator();
    ImGui::Text("Spawning");
    ImGui::SliderInt("Spawn Count", &config.spawnCount, 1, 1000);
    
    if (ImGui::Button("Spawn Ants")) {
        config.spawnAntsRequested = true;
    }
    ImGui::SameLine();
    if (ImGui::Button("Spawn Food")) {
        config.spawnFoodRequested = true;
    }

    ImGui::End();
}