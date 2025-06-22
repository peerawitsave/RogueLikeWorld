#pragma once
#include <vector>
#include <string>
#include <memory>

#include "Item.hpp"
#include "Player.hpp"
#include "Entity.hpp"

namespace PW {

    struct Room {
        int x, y, width, height;

        int centerX() const { return x + width / 2; }
        int centerY() const { return y + height / 2; }

        bool intersects(const Room& other) const {
            return (x <= other.x + other.width && x + width >= other.x &&
                y <= other.y + other.height && y + height >= other.y);
        }
    };

    class Map {
    public:
        Map(int w = 40, int h = 20);

        void draw(const std::vector<std::shared_ptr<Entity>>& enemies,
            const std::vector<Item>& items,
            const Player& player);

        void setTile(int x, int y, char symbol);
        void clear();
        char getTile(int x, int y) const;
        int getWidth() const { return width; }
        int getHeight() const { return height; }

        void generate();
        void generateProcedural();
        const std::vector<Room>& getRooms() const { return rooms; }

    private:
        int width, height;
        std::vector<std::vector<char>> tiles;
        std::vector<Room> rooms;

        void digRoom(const Room& room);
        void digHorizontalTunnel(int x1, int x2, int y);
        void digVerticalTunnel(int y1, int y2, int x);
    };
}
