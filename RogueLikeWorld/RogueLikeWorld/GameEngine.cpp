#include "GameEngine.hpp"
#include "AutoMoveComponent.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

using json = nlohmann::json;

namespace PW {
    GameEngine::GameEngine() {}

    void GameEngine::init() {
        std::ifstream file("settings.json");
        if (!file.is_open()) {
            std::cout << "Could not open settings.json\n";
            return;
        }

        json data;
        try {
            file >> data;
        }
        catch (const std::exception& e) {
            std::cerr << "JSON Parse Error: " << e.what() << "\n";
            return;
        }

        int mapW = data["mapWidth"];
        int mapH = data["mapHeight"];
        int hp = data["player"]["hp"];
        char symbol = data["player"]["symbol"].get<std::string>()[0];

        map = std::make_unique<Map>(mapW, mapH);
        map->generateProcedural();

        player = std::make_unique<Player>();
        player->setHP(hp);
        player->setSymbol(symbol);
        player->setX(data["player"]["x"]);
        player->setY(data["player"]["y"]);

        for (const auto& item : data["items"]) {
            int x = item["x"];
            int y = item["y"];
            char sym = item["symbol"].get<std::string>()[0];
            int effect = item["effect"];
            items.emplace_back(x, y, sym, effect);
        }

        for (const auto& enemy : data["enemies"]) {
            char sym = enemy["symbol"].get<std::string>()[0];
            auto e = std::make_shared<Enemy>(sym);
            e->setX(enemy["x"]);
            e->setY(enemy["y"]);

            if (enemy.contains("components")) {
                for (const auto& compName : enemy["components"]) {
                    if (compName == "AutoMove") {
                        e->addComponent(std::make_shared<AutoMoveComponent>(mapW, mapH));
                    }
                }
            }

            enemies.push_back(e);
        }

        std::cout << "Game started!" << std::endl;
    }

    int GameEngine::handleInput() {
        if (!player || !player->isAlive()) {
            std::cout << "You died!\n";
            return 0;
        }

        std::cout << "Enter command (WASD to move, F to attack, X to exit): ";
        std::string input;
        std::cin >> input;
        if (input.empty()) return 1;
        char command = input[0];

        int x = player->getX();
        int y = player->getY();

        if (command == 'f' || command == 'F') {
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                int ex = (*it)->getX();
                int ey = (*it)->getY();

                bool adjacent =
                    (ex == x && (ey == y - 1 || ey == y + 1)) ||
                    (ey == y && (ex == x - 1 || ex == x + 1));

                if (adjacent) {
                    (*it)->takeDamage(20);
                    std::cout << "You hit " << (*it)->getSymbol() << "! ";
                    if (!(*it)->isAlive()) {
                        std::cout << "It died!\n";
                        it = enemies.erase(it);
                        continue;
                    }
                    else {
                        std::cout << "Remaining HP: " << (*it)->getHP() << "\n";
                    }
                }
                ++it;
            }
            return 1;
        }

        switch (command) {
        case 'w': case 'W': if (y > 0) player->setY(y - 1); break;
        case 's': case 'S': if (y < map->getHeight() - 1) player->setY(y + 1); break;
        case 'a': case 'A': if (x > 0) player->setX(x - 1); break;
        case 'd': case 'D': if (x < map->getWidth() - 1) player->setX(x + 1); break;
        case 'x': case 'X': return 0;
        }

        return 1;
    }

    void GameEngine::update() {
        for (auto it = items.begin(); it != items.end(); ) {
            if (it->getX() == player->getX() && it->getY() == player->getY()) {
                int effect = it->getEffectValue();
                if (effect > 0)
                    std::cout << "You picked up a healing item! +" << effect << " HP\n";
                else
                    std::cout << "You stepped on a trap! " << effect << " HP\n";

                player->takeDamage(-effect);
                it = items.erase(it);
            }
            else {
                ++it;
            }
        }

        for (auto& enemy : enemies) {
            enemy->updateComponents();
            if (enemy->getX() == player->getX() && enemy->getY() == player->getY()) {
                std::cout << "You were attacked by " << enemy->getSymbol() << "!\n";
                player->takeDamage(25);
            }
        }
    }

    void GameEngine::render() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        std::cout << "Rendering game..." << std::endl;

        std::vector<std::shared_ptr<Entity>> entityPtrs;
        for (const auto& e : enemies) {
            entityPtrs.push_back(e);
        }

        map->draw(entityPtrs, items, *player);

        std::cout << "HP: " << player->getHP() << "\n";
        std::cout << "[WASD] Move, [F] Attack, [X] Exit\n";
    }

    void GameEngine::release() {
        std::cout << "Game ended." << std::endl;
    }
}
