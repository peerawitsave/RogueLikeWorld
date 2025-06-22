#include "Player.hpp"

namespace PW {
    Player::Player() : Entity(1, 1, '@'), hp(100) {}

    int Player::getHP() const { return hp; }

    void Player::takeDamage(int amount) {
        hp -= amount;
        if (hp < 0) hp = 0;
    }

    bool Player::isAlive() const {
        return hp > 0;
    }

    void PW::Player::setHP(int value){
        hp = value;
    }

    void PW::Player::setSymbol(char s) {
        symbol = s;
    }
}
