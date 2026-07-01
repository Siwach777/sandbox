#pragma once
#include "ECS/Entity.hpp"
#include "SFML/System/Vector2.hpp"
#include "World/PheromoneGrid.hpp"

class World;

namespace DebugPanel {
    void showStats(float fps, int antCount, int foodCount, sf::Vector2f cameraPos, float zoom, PheromoneGrid m_pheromones);
    void showEntityList(World& world, Entity& selectedEntity);
    void showInspector(World& world, Entity& selectedEntity);
    void showControls();
}