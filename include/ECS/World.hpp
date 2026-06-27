#pragma once
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "Entity.hpp"
#include "Components.hpp"

class World {
    public:
        Entity createEntity();
        void destroyEntity(Entity entity);
        bool isAlive(Entity entity);

        std::unordered_map<Entity, Position> positions;
        std::unordered_map<Entity, Renderable> renderables;

    private:
        Entity m_nextId = 0;
        std::unordered_set<Entity> m_alive;
};