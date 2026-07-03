#pragma once

struct Config {
    const int WIN_WIDTH = 1280;
    const int WIN_HEIGHT = 720;
    const float TILE_SIZE = 5.f;
    const int GRID_WIDTH = WIN_WIDTH/TILE_SIZE;
    const int GRID_HEIGHT = WIN_HEIGHT/TILE_SIZE;

    float speed = TILE_SIZE*15;
    float changeInterval = 1.f;
    float wanderInterval = 1.5f;
    float simSpeed = 1.f;
    int foodPickupAmount = 10;
    int spawnCount = 1;
    float foodDist = TILE_SIZE;
    float foodPickupRange = foodDist;
    float pheromoneEvapRate = 0.1f;
    float pheromoneDepositAmount = 4.f;
    float pheromoneDiffusionRate = 0.0175f;
    int pheromoneDiffuseEveryNTicks = 4;

    // Trigger flags for actions
    bool paused = false;
    bool showGrid = false;
    bool spawnAntsRequested = false;
    bool spawnFoodRequested = false;
    bool showPheromones = true;
};

extern Config config;