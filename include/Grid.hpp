#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Tile.hpp"

class Grid {
    public:
        Grid(int width, int height, float tileSize);

        void setTile(int x, int y, TileType type);
        TileType getTile(int x, int y) const;
        bool inBounds(int x, int y) const;

        void render(sf::RenderWindow& window);
        sf::Vector2i worldToGrid(sf::Vector2f worldPos) const;

        int getWidth() const {return m_width;}
        int getHeight() const {return m_height;}
        float getTileSize() const {return m_tileSize;}

    private:
        void updateTileVeritces(int x, int y, TileType type);

        int m_width;
        int m_height;
        float m_tileSize;

        sf::VertexArray m_vertices;

        std::vector<TileType> m_tiles;
};