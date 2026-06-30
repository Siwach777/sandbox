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

void HUD::render(sf::RenderWindow& window, const HUDData& data) {
    if (!m_initialized) return;
    float bottom = static_cast<float>(window.getSize().y);
    float currentY = bottom - 30.f;

    sf::View worldView = window.getView();
    sf::View screenView(sf::FloatRect({0.f, 0.f}, sf::Vector2f(window.getSize())));
    window.setView(screenView);

    drawText(window, "FPS:    " + std::to_string(static_cast<int>(data.fps)), {10.f, 10.f});
   
    drawText(window, "Camera: " + std::to_string(static_cast<int>(data.cameraPos.x)) + ", "
                                + std::to_string(static_cast<int>(data.cameraPos.y)) 
                                + "\n" + "Zoom: " + std::to_string(data.zoom).substr(0, 4), {10.f, 30.f});

    drawText(window, "Selected: " + data.selectedTileName, {10.f, 70.f});

    if (!data.toolInfo.empty()) {
        drawText(window, data.toolInfo, {10.f, currentY});
        currentY -= 20.f;
    }
    if (!data.hoveredInfo.empty()) {
        drawText(window, data.hoveredInfo, {10.f, currentY});
        currentY -= 20.f;
    }
    if (!data.selectionInfo.empty()) {
        drawText(window, data.selectionInfo, {10.f, currentY});
        currentY -= 20.f;
    }
    // if (!data.stateName.empty()) {
    //     drawText(window, data.stateName, {10.f, currentY});
    //     currentY -= 20.f;
    // }

    window.setView(worldView);
}