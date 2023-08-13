#include "common/Constants.hpp"
#include "view/Game.hpp"
#include "external/loguru.hpp"

int main(int argc, char* argv[]) {

    // Create logs for easier debug
    // loguru::init(argc, argv);
    // loguru::add_file("log.txt", loguru::Append, loguru::Verbosity_INFO);
    LOG_F(INFO, "Game starting.");


    // Main game loop
    Game game;

    if (game.initialize(Constants::GAME_NAME, Constants::SCREEN_WIDTH, Constants::SCREEN_HEIGHT)) {

        while (game.isRunning()) {

            game.handleEvents();

            game.update();

            game.render();

            // Add frame limiting or delay if needed
        }

        game.cleanup();
    }

    // Clean up loguru
    // loguru::shutdown();

    LOG_F(INFO, "Game finishing.");
    return 0;
}
