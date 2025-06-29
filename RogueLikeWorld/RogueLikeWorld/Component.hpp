// Component.hpp
#pragma once

namespace PW {
    class Entity;

    class Component {
    public:
        virtual ~Component() = default;
        virtual void update(Entity& entity) = 0;
    };
}
