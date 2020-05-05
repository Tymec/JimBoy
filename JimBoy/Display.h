#pragma once
#include <iostream>
#include <SDL.h>

static unsigned WIDTH	= 800;
static unsigned HEIGHT	= 600;

class Display {
public:
	Display();
	~Display();
	void Render(void const* buffer, int pitch);

	bool isInitialized();
private:
	SDL_Window* window{}; // SDL Window
	SDL_Renderer* renderer{}; // SDL Renderer
	SDL_Texture* texture{}; // SDL Texture
};

