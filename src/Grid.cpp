#include "Grid.hpp"
#include <vector>

Grid::Grid(int width, int height, float tileSize) 
    : m_width(width)
    , m_height(height)
    , m_tileSize(tileSize)
    , m_tiles(width * height, TileType::Dirt) {}

bool Grid::inBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

void Grid::setTile(int x, int y, TileType type) {
    if (inBounds(x, y)) {
        m_tiles[y * m_width + x] = type;
    }
}

void Grid::render(sf::RenderWindow& window) {
    sf::RectangleShape tileShape({m_tileSize - 1.f, m_tileSize - 1.f});

    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            tileShape.setPosition({x*m_tileSize, y*m_tileSize});
            tileShape.setFillColor(tileColor(m_tiles[y * m_width + x]));
            window.draw(tileShape);
        }
    }
}

TileType Grid::getTile(int x, int y) const {
    if (inBounds (x, y)) {
        return m_tiles[y * m_width + x];
    }
    return TileType::Dirt;
}

sf::Vector2i Grid::worldToGrid(sf::Vector2f worldPos) const {
    return {
        static_cast<int>(worldPos.x / m_tileSize),
        static_cast<int>(worldPos.y / m_tileSize)
    };
}