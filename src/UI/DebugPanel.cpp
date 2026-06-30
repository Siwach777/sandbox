#include "UI/DebugPanel.hpp"
#include "ECS/World.hpp"
#include <imgui.h>
#include <imgui-SFML.h>

void DebugPanel::showStats(float fps, int antCount, int foodCount, sf::Vector2f cameraPos, float zoom) {
    ImGui::Begin("Simulation");

    ImGui::Text("FPS: %.1f", fps);
    ImGui::Separator();
    ImGui::Text("Ants: %d", antCount);
    ImGui::Text("Food sources: %d", foodCount);
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