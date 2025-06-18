#include "GameEngine.hpp"
#include <iostream>

namespace PW {
    GameEngine::GameEngine() {}

    void GameEngine::init() {
        map = std::make_unique<Map>();
        player = std::make_unique<Player>();
        std::cout << "Game started!" << std::endl;
    }

    int GameEngine::handleInput() {
        std::cout << "Enter command (WASD or x to exit): ";
        std::string input;
        std::cin >> input;
        if (input.empty()) return 1;

        char command = input[0];
        int x = player->getX();
        int y = player->getY();

        switch (command) {
            case 'w': case 'W': if (y > 0) player->setY(y - 1); break;
            case 's': case 'S': if (y < map->getHeight() - 1) player->setY(y + 1); break;
            case 'a': case 'A': if (x > 0) player->setX(x - 1); break;
            case 'd': case 'D': if (x < map->getWidth() - 1) player->setX(x + 1); break;
            case 'x': case 'X': return 0;
        }

        return 1;
    }

    void GameEngine::update() {}

    void GameEngine::render() {
#ifdef _WIN32
        system("cls");
#else
        system("clear");
#endif

        std::cout << "Rendering game..." << std::endl;
        map->clear();
        map->setTile(player->getX(), player->getY(), player->getSymbol());
        map->draw();
    }

    void GameEngine::release() {
        std::cout << "Game ended." << std::endl;
    }
}
