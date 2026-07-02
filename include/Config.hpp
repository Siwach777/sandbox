#pragma once

struct Config {
    const int WIN_WIDTH = 1280;
    const int WIN_HEIGHT = 720;
    const float TILE_SIZE = 6.f;
    const int GRID_WIDTH = WIN_WIDTH/TILE_SIZE;
    const int GRID_HEIGHT = WIN_HEIGHT/TILE_SIZE;

    float speed = 50.f;
    float changeInterval = 1.f;
    float wanderInterval = 1.5f;
    float foodDist = 10.f;
    float foodPickupRange = 10.f;
    float simSpeed = 1.f;
    int foodPickupAmount = 10;
    int spawnCount = 10;
    float pheromoneEvapRate = 0.996f;
    float pheromoneDepositAmount = 0.5f;
    float pheromoneDiffusionRate = 0.02f;
    int pheromoneDiffuseEveryNTicks = 4;

    // Trigger flags for actions
    bool paused = false;
    bool showGrid = false;
    bool spawnAntsRequested = false;
    bool spawnFoodRequested = false;
    bool showPheromones = true;
};

extern Config config;