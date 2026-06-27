#pragma once
#include <SFML/Graphics.hpp>

class World;

namespace Systems {
    void render(World& world, sf::RenderWindow& window);
}