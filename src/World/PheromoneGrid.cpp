#include "World/PheromoneGrid.hpp"
#include <algorithm>
#include <cmath>

PheromoneGrid::PheromoneGrid(int width , int height) 
                            :m_width(width), m_height(height)
                            ,m_cells(width*height)
                            ,m_buffer(width*height){}
                        
void PheromoneGrid::deposit(int x, int y, PheromoneType type, float amount) {
    if (inBounds(x, y)) {
        int idx = y * m_width + x;
        if (type == PheromoneType::toHome) {
            m_cells[idx].toHome += amount;
            if (m_cells[idx].toHome > 10.f) m_cells[idx].toHome = 10.f;

            if (m_cells[idx].toHome > m_maxPheromone.toHome) {
                m_maxPheromone.toHome = m_cells[idx].toHome;
            }
        }
        if (type == PheromoneType::toFood) {
            m_cells[idx].toFood += amount;
            if (m_cells[idx].toFood > 10.f) m_cells[idx].toFood = 10.f;

            if (m_cells[idx].toFood > m_maxPheromone.toFood) {
                m_maxPheromone.toFood = m_cells[idx].toFood;
            }
        }
    }
}

bool PheromoneGrid::inBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

float PheromoneGrid::get(int x, int y, PheromoneType type) const {
    if (!inBounds(x, y)) return 0.f;
    return (type == PheromoneType::toHome) ? m_cells[y * m_width + x].toHome : m_cells[y * m_width + x].toFood;
}

void PheromoneGrid::getStrongestNeighbor(int x, int y, PheromoneType type, int& outDx, int& outDy) const {
    outDx = 0;
    outDy = 0;
    float bestValue = 0.f;

    for (int dy = -1; dy <= 1; ++dy) {
        for (int dx = -1; dx <= 1; ++dx) {
            if (dx == 0 && dy == 0) continue;
            float val = get(x + dx, y + dy, type);
            if (val > bestValue) {
                bestValue = val;
                outDx = dx;
                outDy = dy;
            }
        }
    }
}

void PheromoneGrid::evaporate(float ratePerSecond, float dt) {
    float factor = std::exp(-ratePerSecond * dt);
    for (auto& cell : m_cells) {
        cell.toHome *= factor;
        cell.toFood *= factor;
        if (cell.toHome < 0.001f) cell.toHome = 0.f;
        if (cell.toFood < 0.001f) cell.toFood = 0.f;
    }
    m_maxPheromone.toFood = std::max(0.f, m_maxPheromone.toFood * factor);
    m_maxPheromone.toHome = std::max(0.f, m_maxPheromone.toHome * factor);
}

float PheromoneGrid::getMaxValue(PheromoneType type) const {
    return (type == PheromoneType::toHome) ? m_maxPheromone.toHome : m_maxPheromone.toFood;
}

void PheromoneGrid::diffuse(float ratePerSecond, float dt) {
    if (ratePerSecond <= 0.f) return;
    float rate = std::clamp(ratePerSecond * dt, 0.f, 1.f);
    PheromoneCell newMax = {0, 0};

    for (int y = 1; y < m_height - 1; ++y) {
        for (int x = 1; x < m_width -1; ++x) {
            int idx = y * m_width + x;
            PheromoneCell neighborAvg;
            neighborAvg.toFood = (
                m_cells[idx - 1].toFood +
                m_cells[idx + 1].toFood +
                m_cells[idx - m_width].toFood +
                m_cells[idx + m_width].toFood
            ) / 4.f;

            neighborAvg.toHome = (
                m_cells[idx - 1].toHome +
                m_cells[idx + 1].toHome +
                m_cells[idx - m_width].toHome +
                m_cells[idx + m_width].toHome 
            ) / 4.f;

            m_buffer[idx].toFood = m_cells[idx].toFood * (1.f - ratePerSecond) + neighborAvg.toFood * (ratePerSecond);
            if (m_buffer[idx].toFood > newMax.toFood) newMax.toFood = m_buffer[idx].toFood;
            m_buffer[idx].toHome = m_cells[idx].toHome * (1.f - ratePerSecond) + neighborAvg.toHome * ratePerSecond;
            if (m_buffer[idx].toHome > newMax.toHome) newMax.toHome = m_buffer[idx].toHome;
        }
    }

    for (int x = 0; x < m_width; ++x) {
        m_buffer[x].toFood = m_cells[x].toFood;
        m_buffer[(m_height - 1) * m_width + x].toFood = m_cells[(m_height - 1) * m_width + x].toFood;
        newMax.toFood = std::max({newMax.toFood, m_buffer[x].toFood, m_buffer[(m_height - 1) * m_width + x].toFood});
        m_buffer[x].toHome = m_cells[x].toHome;
        m_buffer[(m_height - 1) * m_width + x].toHome = m_cells[(m_height - 1) * m_width + x].toHome;
        newMax.toHome = std::max({newMax.toHome, m_buffer[x].toHome, m_buffer[(m_height - 1) * m_width + x].toHome});
    }

    for (int y = 0; y < m_height; ++y) {
        m_buffer[y*m_width].toFood = m_cells[y*m_width].toFood;
        m_buffer[y * m_width + m_width - 1].toFood = m_cells[y * m_width + m_width - 1].toFood;
        newMax.toFood = std::max({newMax.toFood, m_buffer[y * m_width].toFood, m_buffer[y * m_width + m_width - 1].toFood});
        m_buffer[y*m_width].toHome = m_cells[y*m_width].toHome;
        m_buffer[y * m_width + m_width - 1].toHome = m_cells[y * m_width + m_width - 1].toHome;
        newMax.toHome = std::max({newMax.toHome, m_buffer[y * m_width].toHome, m_buffer[y * m_width + m_width - 1].toHome});
    }

    std::swap(m_cells, m_buffer);
    m_maxPheromone.toFood = newMax.toFood;
    m_maxPheromone.toHome = newMax.toHome;
}