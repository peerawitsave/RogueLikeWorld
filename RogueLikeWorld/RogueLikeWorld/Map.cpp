#include "Map.hpp"
#include <iostream>

namespace PW {
    Map::Map() {
        tiles.resize(height, std::vector<char>(width, '.'));
    }

    void Map::clear() {
        for (auto& row : tiles)
            std::fill(row.begin(), row.end(), '.');
    }

    void Map::setTile(int x, int y, char symbol) {
        if (y >= 0 && y < height && x >= 0 && x < width)
            tiles[y][x] = symbol;
    }

    void Map::draw() {
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                std::cout << tiles[y][x];
            }
            std::cout << std::endl;
        }
    }
}
