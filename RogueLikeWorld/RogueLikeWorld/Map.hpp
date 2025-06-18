#pragma once
#include <vector>

namespace PW {
    class Map {
    public:
        Map();
        void draw();
        void setTile(int x, int y, char symbol);
        void clear(); // clears all tiles to '.'

        int getWidth() const { return width; }
        int getHeight() const { return height; }

    private:
        int width = 10, height = 10;
        std::vector<std::vector<char>> tiles;
    };
}
