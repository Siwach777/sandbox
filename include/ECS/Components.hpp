#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"
#include "Config.hpp"

struct Position {
    float x = 0.f;
    float y = 0.f;
};

struct Renderable {
    float radius = 4.f;
    sf::Color color = sf::Color::Red;
};

struct Velocity {
    float x = 0.f;
    float y = 0.f;
};

struct Wander {
    float speed = config.speed;
    float timer = 0.f;
    float changeInterval = config.changeInterval;
};

struct Food {};

struct foodAmount {
    int amount = (config.spawnCount  * 100) / 10.f;
};

struct Nest {};

struct BelongsToNest {
    Entity nest = INVALID_ENTITY;
};

struct Carrying {
    int amount = 0;
};

enum class AntState {
    Idle,
    Foraging,
    Returning
};

struct AntBehavior {
    AntState state = AntState::Idle;
    float stateTimer = 0.f;
};

struct Heading {
    float angle = 0.f;
};