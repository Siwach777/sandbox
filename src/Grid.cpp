#include "Grid.hpp"
#include "Config.hpp"
#include <vector>

Grid::Grid(int width, int height, float tileSize) 
    : m_width(width)
    , m_height(height)
    , m_tileSize(tileSize)
    , m_tiles(width * height, TileType::Dirt) {
        m_vertices.setPrimitiveType(sf::PrimitiveType::Triangles);
        m_vertices.resize(width*height*6);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                updateTileVeritces(x, y, TileType::Dirt);
            }
        }
    }

bool Grid::inBounds(int x, int y) const {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

void Grid::setTile(int x, int y, TileType type) {
    if (inBounds(x, y)) {
        m_tiles[y * m_width + x] = type;
        updateTileVeritces(x, y, type);
    }
}

void Grid::render(sf::RenderWindow& window) {
    window.draw(m_vertices);
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

void Grid::updateTileVeritces(int x, int y, TileType type) {
    int idx = (y * m_width + x) * 6;

    float px = x * m_tileSize;
    float py = y * m_tileSize;

    float size = config.showGrid ? (m_tileSize - (m_tileSize/10.f)) : m_tileSize;
    sf::Color color = tileColor(type);

    m_vertices[idx + 0].position = {px, py};
    m_vertices[idx + 0].color = color;
    m_vertices[idx + 1].position = {px + size, py};
    m_vertices[idx + 1].color = color;
    m_vertices[idx + 2].position = {px + size, py + size};
    m_vertices[idx + 2].color = color;

    m_vertices[idx + 3].position = {px, py};
    m_vertices[idx + 3].color = color;
    m_vertices[idx + 4].position = {px + size, py + size};
    m_vertices[idx + 4].color = color;
    m_vertices[idx + 5].position = {px, py + size};
    m_vertices[idx + 5].color = color;
}

void Grid::updateAllVertices() {
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            updateTileVeritces(x, y, m_tiles[y * m_width + x]);
        }
    }
}