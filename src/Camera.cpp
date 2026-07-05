#include "Camera.hpp"
#include <algorithm>

Camera::Camera(sf::Vector2f center, sf::Vector2f size) : m_view(size, center), m_zoomLevel(1.f), m_initialSize(size) {}

void Camera::move(sf::Vector2f offset) {
    m_view.move(offset);
}

void Camera::zoom(float factor) {
    float newZoom = m_zoomLevel * factor;
    if (newZoom < 0.2f || newZoom > 5.f) { return; }
    m_zoomLevel = newZoom;

    m_view.zoom(factor);
}

void Camera::constrain(sf::Vector2f worldSize) {
    sf::Vector2f size = m_view.getSize();
    float aspectRatio = size.x / size.y;

    bool resized = false;
    if (size.x > worldSize.x) {
        size.x = worldSize.x;
        size.y = size.x / aspectRatio;
        resized = true;
    }
    if (size.y > worldSize.y) {
        size.y = worldSize.y;
        size.x = size.y * aspectRatio;
        resized = true;
    }

    if (resized) {
        m_view.setSize(size);
        m_zoomLevel = size.x / m_initialSize.x;
    }

    sf::Vector2f center = m_view.getCenter();
    float minX = size.x / 2.f;
    float maxX = worldSize.x - (size.x / 2.f);
    float minY = size.y / 2.f;
    float maxY = worldSize.y - (size.y / 2.f);

    if (maxX < minX) {
        center.x = worldSize.x / 2.f;
    } else {
        center.x = std::clamp(center.x, minX, maxX);
    }

    if (maxY < minY) {
        center.y = worldSize.y / 2.f;
    } else {
        center.y = std::clamp(center.y, minY, maxY);
    }

    m_view.setCenter(center);
}