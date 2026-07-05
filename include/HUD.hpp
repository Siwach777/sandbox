#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

struct HUDData {
    float fps;
    sf::Vector2f cameraPos;
    float zoom;
    const std::string& selectedTileName;
    const std::string& hoveredInfo;
    const std::string& selectionInfo;
    const std::string& toolInfo;
    // const std::string& stateName;
};

class HUD {
    public:
        bool init(const std::string& fontPath);

        void render(sf::RenderWindow& window, const HUDData& data);

    private:
            sf::Font m_font;
            bool m_initialized = false;

            void drawText(sf::RenderWindow& window, 
                          const std::string&str,
                          sf::Vector2f position,
                          uint8_t size = 14);
};