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
        std::unordered_map<Entity, Velocity> velocities;
        std::unordered_map<Entity, Wander> wanders;
        std::unordered_map<Entity, foodAmount> foodAmounts;
        std::unordered_map<Entity, BelongsToNest> belongToNests;
        std::unordered_map<Entity, AntBehavior> antBehaviors;
        std::unordered_map<Entity, Carrying> carryings;
        std::unordered_map<Entity, Heading> headings;

        std::unordered_set<Entity> ants;
        std::unordered_set<Entity> foods;
        std::unordered_set<Entity> nests;


    private:
        Entity m_nextId = 0;
        std::unordered_set<Entity> m_alive;
};