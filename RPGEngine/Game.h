#pragma once

#include "SDL.h"
#include "SDL_image.h"
#include <iostream>
#include <vector>

class Game
{
public:
	Game();
	~Game();

	void Init(const char* title, int xpos, int ypos, int width, int height, bool fullScreen);

	void HandleEvents();
	void Update();
	void Render();
	void Clean();

	bool IsRunning();

	static void AddTile(int srcX, int srcY, int xPos, int yPos);
	static SDL_Renderer* renderer;
	static SDL_Event event;
//	static std::vector<ColliderComponent*> colliders;
	static bool isRunning;
	static SDL_Rect camera;

private:
	SDL_Window* window;
	int counter;

};

