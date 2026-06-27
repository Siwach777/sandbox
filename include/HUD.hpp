#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class HUD {
    public:
        bool init(const std::string& fontPath);

        void render(sf::RenderWindow& window, 
                    float fps, 
                    sf::Vector2f CameraPos,
                    float zoom,
                    const std::string& selectedTileName,
                    const std::string& hoveredInfo);

    private:
            sf::Font m_font;
            bool m_initialized = false;

            void drawText(sf::RenderWindow& window, 
                          const std::string&str,
                          sf::Vector2f position,
                          uint8_t size = 14);
};