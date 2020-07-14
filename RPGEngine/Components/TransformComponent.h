#pragma once
#include "../Utility/Position2D.h"
#include "../Utility/Vector2D.h"


struct TransformComponent 
{
public:
	Position2D Position{};
	Vector2D Velocity{};
	Vector2D Size{};
	float Rotation{};
};