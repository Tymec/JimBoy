#pragma once
#include <iostream>
#include <SDL.h>

static unsigned WIDTH	= 800;
static unsigned HEIGHT	= 600;

class Display {
public:
	Display();
	~Display();
	void render(void const* buffer);

	bool isInitialized();
private:
	SDL_Window* window{}; // SDL Window
	SDL_Renderer* renderer{}; // SDL Renderer
	SDL_Texture* texture{}; // SDL Texture
};

