#include "Map.hpp"
#include "Entity.hpp"
#include "Player.hpp"
#include <iostream>
#include <random>
#include <algorithm>
#include <cmath>

namespace PW {

    Map::Map(int w, int h) : width(w), height(h) {
        tiles.resize(height, std::vector<char>(width, '#'));
        visible.resize(height, std::vector<bool>(width, false));
        explored.resize(height, std::vector<bool>(width, false));
    }

    void Map::draw(const std::vector<std::shared_ptr<Entity>>& entities,
        const std::vector<Item>& items,
        const Player& player) {
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (!isExplored(x, y)) {
                    std::cout << ' ';
                    continue;
                }

                bool drawn = false;
                if (player.getX() == x && player.getY() == y) {
                    std::cout << player.getSymbol();
                    continue;
                }

                if (isVisible(x, y)) {
                    for (const auto& e : entities) {
                        if (e->getX() == x && e->getY() == y) {
                            std::cout << e->getSymbol();
                            drawn = true;
                            break;
                        }
                    }

                    if (!drawn) {
                        for (const auto& item : items) {
                            if (item.getX() == x && item.getY() == y) {
                                std::cout << item.getSymbol();
                                drawn = true;
                                break;
                            }
                        }
                    }

                    if (!drawn) std::cout << tiles[y][x];
                }
                else {
                    std::cout << '.';  // explored but not currently visible
                }
            }
            std::cout << '\n';
        }
    }

    void Map::setTile(int x, int y, char symbol) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            tiles[y][x] = symbol;
        }
    }

    char Map::getTile(int x, int y) const {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return tiles[y][x];
        }
        return '#';
    }

    void Map::clear() {
        for (auto& row : tiles) {
            std::fill(row.begin(), row.end(), '.');
        }
    }

    void Map::digRoom(const Room& room) {
        for (int y = room.y; y < room.y + room.height; ++y) {
            for (int x = room.x; x < room.x + room.width; ++x) {
                setTile(x, y, '.');
            }
        }
    }

    void Map::digHorizontalTunnel(int x1, int x2, int y) {
        for (int x = std::min(x1, x2); x <= std::max(x1, x2); ++x) {
            setTile(x, y, '.');
        }
    }

    void Map::digVerticalTunnel(int y1, int y2, int x) {
        for (int y = std::min(y1, y2); y <= std::max(y1, y2); ++y) {
            setTile(x, y, '.');
        }
    }

    void Map::generateProcedural() {
        std::random_device rd;
        std::mt19937 rng(rd());

        const int MAX_ROOMS = 10;
        const int MIN_ROOM_SIZE = 10;
        const int MAX_ROOM_SIZE = 18;

        rooms.clear();
        tiles.assign(height, std::vector<char>(width, '#'));  // Fill with walls

        std::uniform_int_distribution<int> roomSizeDist(MIN_ROOM_SIZE, MAX_ROOM_SIZE);

        for (int i = 0; i < MAX_ROOMS; ++i) {
            Room newRoom;
            newRoom.width = roomSizeDist(rng);
            newRoom.height = roomSizeDist(rng);

            // Ensure we leave 1 tile space between rooms and edge
            if (width <= newRoom.width + 3 || height <= newRoom.height + 3)
                continue;

            std::uniform_int_distribution<int> posXDist(1, width - newRoom.width - 2);
            std::uniform_int_distribution<int> posYDist(1, height - newRoom.height - 2);
            newRoom.x = posXDist(rng);
            newRoom.y = posYDist(rng);

            bool failed = false;
            for (const auto& room : rooms) {
                // Expand room by 1 unit in all directions to leave space between
                Room expanded = { room.x - 1, room.y - 1, room.width + 2, room.height + 2 };
                if (newRoom.intersects(expanded)) {
                    failed = true;
                    break;
                }
            }

            if (!failed) {
                digRoom(newRoom);

                if (!rooms.empty()) {
                    int prevX = rooms.back().centerX();
                    int prevY = rooms.back().centerY();
                    int newX = newRoom.centerX();
                    int newY = newRoom.centerY();

                    if (rng() % 2) {
                        digHorizontalTunnel(prevX, newX, prevY);
                        digVerticalTunnel(prevY, newY, newX);
                    }
                    else {
                        digVerticalTunnel(prevY, newY, prevX);
                        digHorizontalTunnel(prevX, newX, newY);
                    }
                }

                rooms.push_back(newRoom);
            }
        }
    }


    void Map::generate() {
        generateProcedural();
    }

    void Map::computeFOV(int px, int py, int radius) {
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x)
                visible[y][x] = false;

        for (int dy = -radius; dy <= radius; ++dy) {
            for (int dx = -radius; dx <= radius; ++dx) {
                int nx = px + dx;
                int ny = py + dy;
                if (nx >= 0 && ny >= 0 && nx < width && ny < height) {
                    if (std::hypot(dx, dy) <= radius) {
                        visible[ny][nx] = true;
                        explored[ny][nx] = true;
                    }
                }
            }
        }
    }

    bool Map::isVisible(int x, int y) const {
        return visible[y][x];
    }

    bool Map::isExplored(int x, int y) const {
        return explored[y][x];
    }
}
