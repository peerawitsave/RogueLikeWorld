#include "Enemy.hpp"
#include <cstdlib>
#include <ctime>

namespace PW {
    Enemy::Enemy(char symbol) : Entity(5, 5, symbol), hp(30) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
    }

    void Enemy::moveRandom(int maxX, int maxY) {
        int dx = 0, dy = 0;
        int dir = rand() % 4;
        switch (dir) {
        case 0: dx = -1; break;
        case 1: dx = 1;  break;
        case 2: dy = -1; break;
        case 3: dy = 1;  break;
        }

        int newX = getX() + dx;
        int newY = getY() + dy;

        if (newX >= 0 && newX < maxX && newY >= 0 && newY < maxY) {
            setX(newX);
            setY(newY);
        }
    }

    int Enemy::getHP() const { return hp; }

    void Enemy::takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    bool Enemy::isAlive() const {
        return hp > 0;
    }
}
