#pragma once

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
    protected:
        int posX, posY;
        char symbol;
    };
}
