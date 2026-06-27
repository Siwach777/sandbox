#pragma once
#include <SFML/Graphics.hpp>
#include "Entity.hpp"

struct Position {
    float x = 0.f;
    float y = 0.f;
};

struct Renderable {
    float radius = 4.f;
    sf::Color color = sf::Color::Red;
};