#include <iostream>

#define SDL_MAIN_HANDLED
#include <SDL.h>

#include "mesh.hpp"

static const int width = 1368;
static const int height = 768;

void main_loop(SDL_Renderer*, Mesh&);
void keyboardHandler(SDL_Renderer*, Mesh&);

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

	Mesh mesh(renderer, filename, width / 2, height - 100, 5);

	SDL_SetWindowTitle(window, "Mesh");
	main_loop(renderer, mesh);

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
}

void keyboardHandler(SDL_Renderer* renderer, Mesh& mesh)
{
	const Uint8* kbstate = SDL_GetKeyboardState(NULL);

	if (kbstate[SDL_SCANCODE_W])
	{
		mesh.upScale(10);
	}
	else if (kbstate[SDL_SCANCODE_S])
	{
		mesh.upScale(-10);
	}
	if (kbstate[SDL_SCANCODE_X])
	{
		mesh.rotateX(0.1);
	}
	if (kbstate[SDL_SCANCODE_Y])
	{
		mesh.rotateY(0.1);
	}
	if (kbstate[SDL_SCANCODE_Z])
	{
		mesh.rotateX(0.1);
	}
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
		keyboardHandler(renderer, mesh);

		SDL_RenderPresent(renderer);
	}
}