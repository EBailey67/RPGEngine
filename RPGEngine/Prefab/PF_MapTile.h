#pragma once

#include <entt/entt.hpp>
#include "../Components/TextureComponent.h"
#include "../Components/TransformComponent.h"
#include "../Managers/TextureManager.h"

struct PF_MapTile
{
public:
	void Create(entt::registry& registry, int srcX, int srcY, int xPos, int yPos, const char* filePath);
};



inline void PF_MapTile::Create(entt::registry& registry, int srcX, int srcY, int xPos, int yPos, const char* filePath)
{
	auto mapTileEntity = registry.create();

	SDL_Texture* texture = TextureManager::LoadTexture(filePath);
	if (texture == nullptr)
	{
		std::cout << "Error loading texture :" << filePath << std::endl;
		return;
	}
	
	SDL_Rect srcRect{ srcX, srcY, 32, 32 };

	registry.emplace<TextureComponent>(mapTileEntity, texture, srcRect);

	registry.emplace<TransformComponent>(mapTileEntity,
		Vector2D(xPos, yPos),
		Vector2D(0.0f, 0.0f),
		Vector2D(2.0f, 2.0f), 0.0f);
}