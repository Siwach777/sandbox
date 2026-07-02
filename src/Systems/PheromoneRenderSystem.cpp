#include "Systems/PheromoneRenderSystem.hpp"
#include "World/PheromoneGrid.hpp"
#include <algorithm>

namespace Systems {
    void renderPheromones(PheromoneGrid& grid, sf::RenderWindow& window, float tileSize) {

        static sf::Texture dotTexture;
        static bool textureIntialized = false;

        if (!textureIntialized) {
            sf::Image image(sf::Vector2u(64, 64), sf::Color::Transparent);
            for (unsigned int y = 0; y < 64; ++y) {
                for (unsigned int x = 0; x < 64; ++x) {
                    float dx = static_cast<float>(x) - 31.5f;
                    float dy = static_cast<float>(y) - 31.5f;
                    float dist = std::sqrt(dx*dx + dy*dy);

                    if (dist < 31.5f) {
                        image.setPixel({x, y}, sf::Color(255, 255, 255, 255));
                    } else if (dist < 32.f) {
                        float fraction = 32.f - dist;
                        std::uint8_t alpha = static_cast<uint8_t>(fraction*255.f);
                        image.setPixel({x, y}, sf::Color(255, 255, 255, alpha));
                    }
                }
            }
            (void)dotTexture.loadFromImage(image);
            dotTexture.setSmooth(true);
            textureIntialized = true;
        }

        sf::VertexArray overlay(sf::PrimitiveType::Triangles);

        for (int y = 0; y < grid.getHeight(); ++y) {
            for (int x = 0; x < grid.getWidth(); ++x) {
                float val = grid.get(x, y);
                if (val < 0.01f) continue;

                uint8_t alpha = static_cast<uint8_t>(std::min(val*200.f, 255.f));
                sf::Color color(0, 200, 0, alpha);

               // Calculate cell center
                float cx = x * tileSize + tileSize / 2.f;
                float cy = y * tileSize + tileSize / 2.f;

                // Scale the dot's size based on the pheromone strength
                float maxVal = 2.f; // Reference value for full size
                float sizeScale = std::min(val / maxVal, 1.0f);
                float size = (tileSize * 1.5f) * sizeScale; 

                if (size < 0.5f) continue; // Skip if too small to see

                float halfSize = size / 2.f;

                // 4 corners of the quad
                sf::Vector2f tl = {cx - halfSize, cy - halfSize};
                sf::Vector2f tr = {cx + halfSize, cy - halfSize};
                sf::Vector2f br = {cx + halfSize, cy + halfSize};
                sf::Vector2f bl = {cx - halfSize, cy + halfSize};

                // Map texture coordinates (0..64)
                sf::Vector2f t_tl = {0.f, 0.f};
                sf::Vector2f t_tr = {64.f, 0.f};
                sf::Vector2f t_br = {64.f, 64.f};
                sf::Vector2f t_bl = {0.f, 64.f};

                // Triangle 1
                overlay.append({tl, color, t_tl});
                overlay.append({tr, color, t_tr});
                overlay.append({br, color, t_br});

                // Triangle 2
                overlay.append({tl, color, t_tl});
                overlay.append({br, color, t_br});
                overlay.append({bl, color, t_bl});
            }
        }

        // Draw all dots in a single batch
        sf::RenderStates states(&dotTexture);
        window.draw(overlay, states);
    }
}