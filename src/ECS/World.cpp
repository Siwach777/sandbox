#include "ECS/World.hpp"

Entity World::createEntity() {
    Entity id = m_nextId++;
    m_alive.insert(id);
    return id;
}

void World::destroyEntity(Entity entity) {
    m_alive.erase(entity);

    positions.erase(entity);
    renderables.erase(entity);
    velocities.erase(entity);
    wanders.erase(entity);
    foodAmounts.erase(entity);
    belongToNests.erase(entity);
    antBehaviors.erase(entity);

    ants.erase(entity);
    foods.erase(entity);
    nests.erase(entity);
}

bool World::isAlive(Entity entity) {
    return m_alive.count(entity) > 0;
}