#pragma once
#include <entt/entity/registry.hpp>

#include "../Components/InputComponent.h"
#include "../Components/TransformComponent.h"

class InputSystem
{
public:
	void Update(entt::registry& Registry, float deltaTime);
private:

};