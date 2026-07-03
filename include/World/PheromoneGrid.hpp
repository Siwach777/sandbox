#pragma once
#include <vector>

enum class PheromoneType {
    toHome,
    toFood
};

struct PheromoneCell {
    float toHome = 0.f;
    float toFood = 0.f;
};

class PheromoneGrid {
    public: 
        PheromoneGrid(int width, int height);

        void deposit(int x, int y, PheromoneType type, float amount);
        void diffuse(float ratePerSecond, float dt);
        float get(int x, int y, PheromoneType type) const;

        void getStrongestNeighbor(int x, int y, PheromoneType type, int& outDx, int& outDy) const;

        void evaporate(float ratePerSecond, float dt);

        bool inBounds(int x, int y) const;
        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }
        float getMaxValue(PheromoneType type) const;
        
    private:
        int m_width;
        int m_height;

        PheromoneCell m_maxPheromone;
        std::vector<PheromoneCell> m_cells;
        std::vector<PheromoneCell> m_buffer;
};