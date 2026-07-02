#include "World/PheromoneGrid.hpp"
#include <algorithm>

PheromoneGrid::PheromoneGrid(int width , int height) 
                            :m_width(width), m_height(height)
                            ,m_cells(width*height, 0.f)
                            ,m_buffer(width*height, 0.f){}
                        
void PheromoneGrid::deposit(int x, int y, float amount) {
    if (inBounds(x, y)) {
        int idx = y * m_width + x;
        m_cells[idx] += amount;

        if (m_cells[idx] > 10.f) m_cells[idx] = 10.f;

        if (m_cells[idx] > m_maxPheromone) {
            m_maxPheromone = m_cells[idx];
        }
    }
}

bool PheromoneGrid::inBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

float PheromoneGrid::get(int x, int y) const {
    if (!inBounds(x, y)) return 0.f;
    return m_cells[y * m_width + x];
}

void PheromoneGrid::getStrongestNeighbor(int x, int y, int& outDx, int& outDy) const {
    outDx = 0;
    outDy = 0;
    float bestValue = 0.f;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            float val = get(x + dx, y + dy);
            if (val > bestValue) {
                bestValue = val;
                outDx = dx;
                outDy = dy;
            }
        }
    }
}

void PheromoneGrid::evaporate(float factor) {
    for (float& cell : m_cells) {
        cell *= factor;
        if (cell < 0.001f) cell = 0.f;
    }
    m_maxPheromone *= factor;
    if (m_maxPheromone < 0.001f) m_maxPheromone = 0.f;
}

float PheromoneGrid::getMaxValue() const {
    return m_maxPheromone;
}

void PheromoneGrid::diffuse(float rate) {
    if (rate <= 0.f) return;
    float newMax = 0.f;

    for (int y = 1; y < m_height - 1; ++y) {
        for (int x = 1; x < m_width -1; ++x) {
            int idx = y * m_width + x;
            float neighborAvg = (
                m_cells[idx - 1] +
                m_cells[idx + 1] +
                m_cells[idx - m_width] +
                m_cells[idx + m_width]
            ) / 4.f;

            m_buffer[idx] = m_cells[idx] * (1.f - rate) + neighborAvg * (rate);
            if (m_buffer[idx] > newMax) newMax = m_buffer[idx];
        }
    }

    for (int x = 0; x < m_width; ++x) {
        m_buffer[x] = m_cells[x];
        m_buffer[(m_height - 1) * m_width + x] = m_cells[(m_height - 1) * m_width + x];
        newMax = std::max({newMax, m_buffer[x], m_buffer[(m_height - 1) * m_width + x]});
    }

    for (int y = 0; y < m_height; ++y) {
        m_buffer[y*m_width] = m_cells[y*m_width];
        m_buffer[y * m_width + m_width - 1] = m_cells[y * m_width + m_width - 1];
        newMax = std::max({newMax, m_buffer[y * m_width], m_buffer[y * m_width + m_width - 1]});
    }

    std::swap(m_cells, m_buffer);
    m_maxPheromone = newMax;
}