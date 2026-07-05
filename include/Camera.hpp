#pragma once
#include <SFML/Graphics.hpp>

class Camera {
    public:
        Camera(sf::Vector2f center, sf::Vector2f size);

        void move(sf::Vector2f offset);
        void zoom(float factor);
        void constrain(sf::Vector2f worldSize);

        const sf::View& getView() const { return m_view; }
        float getZoom() const { return m_zoomLevel; }
        sf::Vector2f getCenter() const { return m_view.getCenter(); }

    private:
        sf::View m_view;
        float m_zoomLevel;
        sf::Vector2f m_initialSize;
};