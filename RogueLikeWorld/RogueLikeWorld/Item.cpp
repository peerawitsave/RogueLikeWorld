#include "Item.hpp"

namespace PW {
    Item::Item(int x, int y, char symbol, int value)
        : x(x), y(y), symbol(symbol), effectValue(value) {}

    int Item::getX() const { return x; }
    int Item::getY() const { return y; }
    char Item::getSymbol() const { return symbol; }
    int Item::getEffectValue() const { return effectValue; }
}
