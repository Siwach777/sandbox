#include "Systems/PheromoneRenderSystem.hpp"
#include "World/PheromoneGrid.hpp"
#include <algorithm>

namespace Systems {
    void renderPheromones(PheromoneGrid& grid, sf::RenderWindow& window, float tileSize) {
        sf::VertexArray overlay(sf::PrimitiveType::Triangles);

        for (int y = 0; y < grid.getHeight(); ++y) {
            for (int x = 0; x < grid.getWidth(); ++x) {
                float val = grid.get(x, y);
                if (val < 0.01f) continue;

                uint8_t alpha = static_cast<uint8_t>(std::min(val*200.f, 255.f));
                sf::Color color(0, 200, 0, alpha);

                float px = x * tileSize;
                float py = y * tileSize;

                overlay.append({{px, py}, color});
                overlay.append({{px + tileSize, py}, color});
                overlay.append({{px + tileSize, py + tileSize}, color});

                overlay.append({{px, py}, color});
                overlay.append({{px + tileSize, py + tileSize}, color});
                overlay.append({{px, py + tileSize}, color});
            }
        }
        window.draw(overlay);
    }
}