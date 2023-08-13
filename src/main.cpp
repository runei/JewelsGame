#include "common/Constants.hpp"
#include "view/Game.hpp"

int main(int argc, char* argv[]) {

    Game game;

    if (game.initialize(Constants::GAME_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT)) {

        while (game.isRunning()) {

            game.handleEvents();

            game.update();

            game.render();

            // Add frame limiting or delay if needed
        }

        game.cleanup();
    }

    return 0;
}
