#pragma once
#include "Component.hpp"
#include <vector>
#include <memory>

namespace PW {

    class Entity {
    public:
        Entity();
        Entity(int x, int y, char symbol);

        int getX() const;
        int getY() const;
        char getSymbol() const;
        void setX(int x);
        void setY(int y);
        void setSymbol(char s);

        int getHP() const;
        void setHP(int hp);
        void takeDamage(int dmg);
        bool isAlive() const;

        void addComponent(std::shared_ptr<PW::Component> comp);
        void updateComponents();

    protected:
        int posX, posY;
        char symbol = '@';
        int hp = 100;

        std::vector<std::shared_ptr<PW::Component>> components;
    };
}
