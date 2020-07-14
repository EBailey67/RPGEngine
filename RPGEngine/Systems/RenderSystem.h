#pragma once

#include <iostream>
#include <SDL.h>
#include <entt/entt.hpp>
#include "../Components/TransformComponent.h"
#include "../Components/TextureComponent.h"
#include "../Managers/TextureManager.h"

class RenderSystem
{
public:
	void Update(entt::registry& Registry);
	void Draw(entt::registry& Registry, const SDL_Renderer* renderer);
private:

};

