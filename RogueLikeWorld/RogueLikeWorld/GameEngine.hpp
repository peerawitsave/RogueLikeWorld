    #pragma once
    #include "json.hpp"
    #include "fstream"
    #include <memory>
    #include "Map.hpp"
    #include "Player.hpp"
    #include "Enemy.hpp"
    #include "Item.hpp"
    #include <vector>
    using json = nlohmann::json;

    namespace PW {

        class GameEngine {
        public:
            GameEngine();
            void init();
            int handleInput();
            void update();
            void render();
            void release();
        private:
            std::unique_ptr<Map> map;
            std::unique_ptr<Player> player;
            std::vector<std::shared_ptr<Enemy>> enemies;
            std::vector<Item> items;
        };
    }
