#pragma once
#include <SFML/Graphics.hpp>

enum class TileType : uint8_t {
    Dirt,
    Stone,
    Sand,
    Water,
    Grass,
    Wall,
    COUNT
};

inline sf::Color tileColor(TileType type) {
    switch (type) {
        case TileType::Dirt:  return sf::Color(139, 115, 85);
        case TileType::Stone: return sf::Color(136, 140, 141);
        case TileType::Sand:  return sf::Color(210, 190, 140);
        case TileType::Water: return sf::Color(64, 120, 190);
        case TileType::Grass: return sf::Color(86, 152, 59);
        case TileType::Wall:  return sf::Color(60, 60, 65);
        default:              return sf::Color::Magenta;
    }
}

inline const char* tileName(TileType type) {
    switch (type) {
        case TileType::Dirt:  return "Dirt";
        case TileType::Stone: return "Stone";
        case TileType::Sand:  return "Sand";
        case TileType::Water: return "Water";
        case TileType::Grass: return "Grass";
        case TileType::Wall:  return "Wall";
        default:              return "Unknown";
    }
}