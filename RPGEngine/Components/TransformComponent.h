#pragma once
#include "../Utility/Vector2d.h"

struct TransformComponent 
{
public:
	Vector2D Position{};
	Vector2D Velocity{};
	Vector2D Size{};
	float Rotation{};
};