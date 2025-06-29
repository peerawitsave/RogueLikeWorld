#pragma once
#include "Entity.hpp"

namespace PW {
    class Player : public Entity {
    public:
        Player();
        int getHP() const;
        void takeDamage(int amount);
        bool isAlive() const;

        void setHP(int value);
        void setSymbol(char s);

        int& getMutableX() { return posX; }
        int& getMutableY() { return posY; }

    private:
        int hp;
    };
}
