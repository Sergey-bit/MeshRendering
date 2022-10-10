#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "mesh.hpp"

static const int width = 1368;
static const int height = 768;

void main_loop(SDL_Renderer*, Mesh&);

int main()
{
	SDL_SetMainReady();

	SDL_Renderer* renderer = nullptr;
	SDL_Window* window = nullptr;

	std::string filename;

	std::cout << "Enter a path to the file >> ";
	std::cin >> filename;

	if (SDL_Init(SDL_INIT_VIDEO))
	{
		std::cerr << "FAIELD TO INITIALIZE SDL: " << SDL_GetError() << std::endl;
		return -1;
	}
	if (SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_INPUT_FOCUS, &window, &renderer))
	{
		std::cerr << "Failed to create renderer and window: " << SDL_GetError() << std::endl;
		return -1;
	}

	Mesh mesh(renderer, filename, width / 2, height - 100, 400);

	SDL_SetWindowTitle(window, "Mesh");
	main_loop(renderer, mesh);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

void main_loop(SDL_Renderer* renderer, Mesh& mesh)
{
	SDL_Event event;

	while (true)
	{
		if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
		{
			break;
		}

		SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
		SDL_RenderClear(renderer);

		// Mesh handling
		mesh.draw();
		mesh.rotateY(0.005);

		SDL_RenderPresent(renderer);
	}
}