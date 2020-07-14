#pragma once
#include <vector>
#include <SDL.h>

struct InputComponent
{
public:
	std::vector<SDL_Scancode> KeyList{};
};