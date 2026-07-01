#pragma once

struct Config {
    const int WIN_WIDTH = 1280;
    const int WIN_HEIGHT = 720;
    const int GRID_WIDTH = WIN_WIDTH/10;
    const int GRID_HEIGHT = WIN_HEIGHT/10;

    float speed = 50.f;
    float changeInterval = 1.f;
    float wanderInterval = 1.5f;
    float foodDist = 10.f;
    float foodPickupRange = 10.f;
    float simSpeed = 1.f;
    int foodPickupAmount = 10;
    int spawnCount = 10;
    bool paused = false;
    float pheromoneEvapRate = 0.995f;
    float pheromoneDepositAmount = 0.5f;

    // Trigger flags for actions
    bool spawnAntsRequested = false;
    bool spawnFoodRequested = false;
    bool showPheromones = true;
};

extern Config config;