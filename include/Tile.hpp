#pragma once
#include <SFML/Graphics.hpp>

enum class TileType : uint8_t {
    Dirt,
    Stone,
    Sand,
    Water,
    COUNT
};

inline sf::Color tileColor(TileType type) {
    switch (type) {
        case TileType::Dirt:  return sf::Color(139, 115, 85);
        case TileType::Stone: return sf::Color(136, 140, 141);
        case TileType::Sand:  return sf::Color(210, 190, 140);
        case TileType::Water: return sf::Color(64, 120, 190);
        default:              return sf::Color::Magenta;
    }
}