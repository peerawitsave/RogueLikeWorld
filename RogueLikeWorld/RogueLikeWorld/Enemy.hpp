#pragma once
#include "Entity.hpp"
#include <string>

namespace PW {
    class Enemy : public Entity {
    public:
        Enemy(char symbol = 'E');
        void moveRandom(int maxX, int maxY);

        int getHP() const;
        void takeDamage(int amount);
        bool isAlive() const;

    private:
        int hp;
    };
}