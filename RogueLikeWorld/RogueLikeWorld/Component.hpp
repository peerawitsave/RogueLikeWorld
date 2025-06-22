#pragma once
#include "Entity.hpp"

namespace PW {
    class Entity;

    class Component {
    public:
        virtual ~Component() = default;
        virtual void update(Entity& owner) = 0;
    };
}
