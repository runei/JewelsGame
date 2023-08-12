#include <SDL.h>
#include "src/common/Constants.hpp"
#include "src/view/SDLRenderer.hpp"
#include "src/view/GameView.hpp"
#include "src/viewmodel/GameViewModel.hpp"

int main(int argc, char* argv[]) {
	
	// Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		return 1;
	} 

	// Create SDL window and game components
    SDL_Window* window = SDL_CreateWindow("Jewel Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	SDLRenderer sdlRenderer(renderer);
	GameViewModel gameViewModel;

	sdlRenderer.Initialize();

	GameView gameView(sdlRenderer);

	// main loop
	while (true) {
		// handle events #TODO

		gameViewModel.Update();

		sdlRenderer.Clear();

		gameView.Render(gameViewModel);
	}

	// Clean and quit SDL
    sdlRenderer.Cleanup();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}