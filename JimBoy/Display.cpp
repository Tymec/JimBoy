#include "Display.h"

Display::Display() {
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		std::cout << "Could not initialize SDL2: " << SDL_GetError() << std::endl;
		return;
	}

	window = SDL_CreateWindow("JimBoy", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		std::cout << "Could not create window: " << SDL_GetError() << std::endl;
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (renderer == NULL) {
		std::cout << "Could not create renderer: " << SDL_GetError() << std::endl;
		return;
	}

	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, WIDTH, HEIGHT);
}

Display::~Display() {
	SDL_DestroyTexture(texture);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Display::Render(void const* buffer, int pitch) {
	SDL_UpdateTexture(texture, nullptr, buffer, pitch);
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, nullptr, nullptr);
	SDL_RenderPresent(renderer);
}

bool Display::isInitialized() {
	return (renderer != NULL);
}