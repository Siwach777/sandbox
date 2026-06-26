#include "Camera.hpp"
#include <algorithm>

Camera::Camera(sf::Vector2f center, sf::Vector2f size) : m_view(size, center), m_zoomLevel(1.f) {}

void Camera::move(sf::Vector2f offset) {
    m_view.move(offset);
}

void Camera::zoom(float factor) {
    float newZoom = m_zoomLevel * factor;
    if (newZoom < 0.2f || newZoom > 5.f) { return; }
    m_zoomLevel = newZoom;

    m_view.zoom(factor);
}