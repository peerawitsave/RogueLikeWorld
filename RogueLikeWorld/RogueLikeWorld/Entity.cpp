#include "Entity.hpp"

namespace PW {
    
    Entity::Entity()
        : posX(0), posY(0), symbol('@'), hp(100) {}

    Entity::Entity(int x, int y, char symbol)
        : posX(x), posY(y), symbol(symbol), hp(100) {}

    int Entity::getX() const { return posX; }
    int Entity::getY() const { return posY; }
    char Entity::getSymbol() const { return symbol; }
    void Entity::setX(int x) { posX = x; }
    void Entity::setY(int y) { posY = y; }
    void Entity::setSymbol(char s) { symbol = s; }

    int Entity::getHP() const { return hp; }
    void Entity::setHP(int h) { hp = h; }
    void Entity::takeDamage(int dmg) { hp -= dmg; }
    bool Entity::isAlive() const { return hp > 0; }

    void Entity::addComponent(std::shared_ptr<Component> comp) {
        components.push_back(comp);
    }

    void Entity::updateComponents() {
        for (auto& comp : components) {
            comp->update(*this);
        }
    }
}
