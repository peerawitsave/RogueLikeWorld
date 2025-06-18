#include <iostream>
#include <memory>
#include "GameEngine.hpp"

using namespace PW;

int main() {
    std::unique_ptr<GameEngine> gameEngine = std::make_unique<GameEngine>();
    gameEngine->init();

    while (true) {
        char result = gameEngine->handleInput();
        if (result == 0) break;

        gameEngine->update();
        gameEngine->render();
    }

    gameEngine->release();
    return 0;
}
