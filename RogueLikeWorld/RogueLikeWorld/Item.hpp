#pragma once

namespace PW {
    class Item {
    public:
        Item(int x, int y, char symbol, int effectValue);

        int getX() const;
        int getY() const;
        char getSymbol() const;
        int getEffectValue() const;

    private:
        int x, y;
        char symbol;
        int effectValue; // positive = heal, negative = damage
    };
}
#pragma once
