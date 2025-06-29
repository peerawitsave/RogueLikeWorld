#pragma once
#include "Component.hpp"
#include "Map.hpp"

namespace PW {
    class AutoMoveComponent : public Component {
    public:
        AutoMoveComponent(Map* map);
        void update(Entity& entity) override;
    private:
        Map* map;
    };
}
