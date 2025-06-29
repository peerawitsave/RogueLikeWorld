#include "AutoMoveComponent.hpp"
#include "Entity.hpp"
#include "Component.hpp"
#include <cstdlib>

namespace PW {

    AutoMoveComponent::AutoMoveComponent(Map* mapPtr) : map(mapPtr) {}

    void AutoMoveComponent::update(Entity& entity) {
        int x = entity.getX();
        int y = entity.getY();

        // Randomize movement direction
        int directions[4][2] = {
            { 0, -1 },  // up
            { 0,  1 },  // down
            { -1, 0 },  // left
            { 1,  0 }   // right
        };

        int startDir = rand() % 4;

        for (int i = 0; i < 4; ++i) {
            int dirIndex = (startDir + i) % 4;
            int newX = x + directions[dirIndex][0];
            int newY = y + directions[dirIndex][1];

            if (newX >= 0 && newX < map->getWidth() &&
                newY >= 0 && newY < map->getHeight() &&
                map->getTile(newX, newY) == '.') {
                entity.setX(newX);
                entity.setY(newY);
                break;
            }
        }
    }
}
