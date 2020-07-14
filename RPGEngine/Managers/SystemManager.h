#pragma once

#include <entt\entt.hpp>
#include <SDL.h>
#include <SDL_ttf.h>


class SystemManager
{
public:
	void Activate();
	void Update(SDL_Window* Window);

	static SDL_Renderer* renderer;
	static SDL_Event event;

private:
	bool isRunning;

	entt::registry Registry;

	SDL_Window* window;
	TTF_Font* uiFont;
	SDL_Texture* uiTexture;
};
