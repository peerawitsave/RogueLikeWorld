#include "Entity.hpp"

namespace PW {
    Entity::Entity() : posX(0), posY(0), symbol('X') {}
    Entity::Entity(int x, int y, char symbol)
        : posX(x), posY(y), symbol(symbol) {}

    int Entity::getX() const { return posX; }
    int Entity::getY() const { return posY; }
    char Entity::getSymbol() const { return symbol; }
    void Entity::setX(int x) { posX = x; }
    void Entity::setY(int y) { posY = y; }
}
