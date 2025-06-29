#include "GameEngine.hpp"
#include "AutoMoveComponent.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>

using json = nlohmann::json;

namespace PW {

    bool isPassable(const Map& map, int x, int y) {
        return map.getTile(x, y) != '#';
    }

    int distance(int x1, int y1, int x2, int y2) {
        return std::abs(x1 - x2) + std::abs(y1 - y2);
    }

    std::pair<int, int> getRandomFreePosition(const Map& map) {
        int x, y;
        do {
            x = rand() % map.getWidth();
            y = rand() % map.getHeight();
        } while (!isPassable(map, x, y));
        return std::make_pair(x, y);
    }

    GameEngine::GameEngine() {}

    void GameEngine::init() {
        std::srand(static_cast<unsigned>(std::time(nullptr)));

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

        // Setup player
        player = std::make_unique<Player>();
        player->setHP(hp);
        player->setSymbol(symbol);
        auto playerPos = getRandomFreePosition(*map);
        player->setX(playerPos.first);
        player->setY(playerPos.second);

        // Spawn items
        for (const auto& item : data["items"]) {
            auto pos = getRandomFreePosition(*map);
            char sym = item["symbol"].get<std::string>()[0];
            int effect = item["effect"];
            items.emplace_back(pos.first, pos.second, sym, effect);
        }

        // Spawn enemies far from player
        for (const auto& enemy : data["enemies"]) {
            std::pair<int, int> pos;
            do {
                pos = getRandomFreePosition(*map);
            } while (distance(pos.first, pos.second, playerPos.first, playerPos.second) < 6);

            char sym = enemy["symbol"].get<std::string>()[0];
            auto e = std::make_shared<Enemy>(sym);
            e->setX(pos.first);
            e->setY(pos.second);

            if (enemy.contains("components")) {
                for (const auto& compName : enemy["components"]) {
                    if (compName == "AutoMove") {
                        e->addComponent(std::make_shared<AutoMoveComponent>(map.get()));
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

        if (enemies.empty()) {
            std::cout << "You killed all enemies! Victory!\n";
            return 0;
        }

        std::cout << "Enter command (WASD to move, F to attack, X to exit): ";
        std::string input;
        std::cin >> input;
        if (input.empty()) return 1;
        char command = input[0];

        int x = player->getX();
        int y = player->getY();
        int newX = x;
        int newY = y;

        switch (command) {
        case 'w': case 'W': newY = y - 1; break;
        case 's': case 'S': newY = y + 1; break;
        case 'a': case 'A': newX = x - 1; break;
        case 'd': case 'D': newX = x + 1; break;
        case 'x': case 'X': return 0;
        case 'f': case 'F':
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
        default: return 1;
        }

        if (newX >= 0 && newX < map->getWidth() &&
            newY >= 0 && newY < map->getHeight() &&
            isPassable(*map, newX, newY)) {
            player->setX(newX);
            player->setY(newY);
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
