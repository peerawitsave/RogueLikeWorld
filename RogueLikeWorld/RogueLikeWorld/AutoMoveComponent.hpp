#pragma once
#include "Component.hpp"
#include <cstdlib> // for rand
#include <ctime>   // for seeding

namespace PW {
    class AutoMoveComponent : public Component {
    public:
        AutoMoveComponent(int mapW, int mapH) : width(mapW), height(mapH) {
            std::srand(static_cast<unsigned>(std::time(nullptr)));
        }

        void update(Entity& owner) override {
            int dx = (std::rand() % 3) - 1; // -1, 0, or 1
            int dy = (std::rand() % 3) - 1;

            int newX = owner.getX() + dx;
            int newY = owner.getY() + dy;

            if (newX >= 0 && newX < width) owner.setX(newX);
            if (newY >= 0 && newY < height) owner.setY(newY);
        }

    private:
        int width, height;
    };
}
