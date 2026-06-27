#include "HUD.hpp"

bool HUD::init(const std::string& fontPath) {
    if (!m_font.openFromFile(fontPath)) { return false; }
    m_initialized = true;
    return true;
}

void HUD::drawText(sf::RenderWindow& window, const std::string&str, sf::Vector2f position, uint8_t size) {
    sf::Text text(m_font, str, size);
    text.setPosition(position);
    text.setFillColor(sf::Color(220, 220, 220));
    window.draw(text);
}

void HUD::render(sf::RenderWindow& window, float fps, sf::Vector2f cameraPos, float zoom, const std::string& selectedTileName, const std::string& hoveredInfo) {
    if (!m_initialized) return;

    sf::View worldView = window.getView();
    sf::View screenView(sf::FloatRect({0.f, 0.f}, sf::Vector2f(window.getSize())));
    window.setView(screenView);

    drawText(window, "FPS:    " + std::to_string(static_cast<int>(fps)), {10.f, 10.f});
   
    drawText(window, "Camera: " + std::to_string(static_cast<int>(cameraPos.x)) + ", "
                                + std::to_string(static_cast<int>(cameraPos.y)) 
                                + "Zoom: " + std::to_string(zoom).substr(0, 4), {10.f, 30.f});

    drawText(window, "Selected: " + selectedTileName, {10.f, 50.f});

    if (!hoveredInfo.empty()) {
        float bottoom = static_cast<float>(window.getSize().y);
        drawText(window, hoveredInfo, {10.f, bottoom - 30.f});
    }

    window.setView(worldView);
}