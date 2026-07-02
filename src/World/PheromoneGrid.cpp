#include "World/PheromoneGrid.hpp"
#include <algorithm>

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

            if (m_cells[idx].toHome > m_maxToHomePheromone) {
                m_maxToHomePheromone = m_cells[idx].toHome;
            }
        }
        if (type == PheromoneType::toFood) {
            m_cells[idx].toFood += amount;
            if (m_cells[idx].toFood > 10.f) m_cells[idx].toFood = 10.f;

            if (m_cells[idx].toFood > m_maxToFoodPheromone) {
                m_maxToFoodPheromone = m_cells[idx].toFood;
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

void PheromoneGrid::evaporate(float factor) {
    for (auto& cell : m_cells) {
        cell.toHome *= factor;
        cell.toFood *= factor;
        if (cell.toHome < 0.001f) cell.toHome = 0.f;
        if (cell.toFood < 0.001f) cell.toFood = 0.f;
    }
    m_maxToHomePheromone = std::max(0.f, m_maxToHomePheromone * factor);
    m_maxToFoodPheromone = std::max(0.f, m_maxToFoodPheromone * factor);
}

float PheromoneGrid::getMaxValue(PheromoneType type) const {
    return (type == PheromoneType::toHome) ? m_maxToHomePheromone : m_maxToFoodPheromone;
}

// void PheromoneGrid::diffuse(float rate) {
//     if (rate <= 0.f) return;
//     float newMax = 0.f;

//     for (int y = 1; y < m_height - 1; ++y) {
//         for (int x = 1; x < m_width -1; ++x) {
//             int idx = y * m_width + x;
//             float neighborAvg = (
//                 m_cells[idx - 1] +
//                 m_cells[idx + 1] +
//                 m_cells[idx - m_width] +
//                 m_cells[idx + m_width]
//             ) / 4.f;

//             m_buffer[idx] = m_cells[idx] * (1.f - rate) + neighborAvg * (rate);
//             if (m_buffer[idx] > newMax) newMax = m_buffer[idx];
//         }
//     }

//     for (int x = 0; x < m_width; ++x) {
//         m_buffer[x] = m_cells[x];
//         m_buffer[(m_height - 1) * m_width + x] = m_cells[(m_height - 1) * m_width + x];
//         newMax = std::max({newMax, m_buffer[x], m_buffer[(m_height - 1) * m_width + x]});
//     }

//     for (int y = 0; y < m_height; ++y) {
//         m_buffer[y*m_width] = m_cells[y*m_width];
//         m_buffer[y * m_width + m_width - 1] = m_cells[y * m_width + m_width - 1];
//         newMax = std::max({newMax, m_buffer[y * m_width], m_buffer[y * m_width + m_width - 1]});
//     }

//     std::swap(m_cells, m_buffer);
//     m_maxPheromone = newMax;
// }