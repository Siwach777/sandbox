#pragma once
#include <vector>

class PheromoneGrid {
    public: 
        PheromoneGrid(int width, int height);

        void deposit(int x, int y, float amount);
        float get(int x, int y) const;

        void getStrongestNeighbor(int x, int y, int& outDx, int& outDy) const;

        void evaporate(float factor);

        bool inBounds(int x, int y) const;
        int getWidth() const { return m_width; }
        int getHeight() const { return m_height; }
        float getMaxValue() const;
        
    private:
        int m_width;
        int m_height;
        std::vector<float> m_cells;
};