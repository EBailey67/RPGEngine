#include "InputSystem.h"

/*
 * Function:  Update
 * --------------------
 * This checks the list of keys for the entity and if it matches with one that matters,
 * then do something. In this case, it's to apply opposite force.
 *
 * Registry: This list of all entities.
 * deltaTime: This is the constant time half-step
 *
 */
void InputSystem::Update(entt::registry& Registry, float DeltaTime)
{
	// For every entity with a Transform component and InputSystem component...
	Registry.view<TransformComponent, InputComponent>().each([&](auto Entity, TransformComponent& TransformComponent, InputComponent& InputComponent)
		{
			const Uint8* state = SDL_GetKeyboardState(nullptr);
			// There's checks every key that is registered for that entity.
			// The entities actions are mutually shared by the same input.
			for (auto& CurrentKey : InputComponent.KeyList)
			{
				//  If you're holding right, then accelerate to the right.
				if (state[CurrentKey] && CurrentKey == SDL_SCANCODE_RIGHT) 
				{
//					TransformComponent.Velocity.x += _ACCELERATION * DeltaTime;
				}
				else if (state[CurrentKey] && CurrentKey == SDL_SCANCODE_LEFT) 
				{
//					TransformComponent.Velocity.x += -_ACCELERATION * DeltaTime;
				}

				if (state[CurrentKey] && CurrentKey == SDL_SCANCODE_SPACE)
				{
					// Only jump if the vertical position of the entity is touching the "ground".
					// Y increases by going downward.
					// This should cause the object to shoot up only once when touching the ground.
					//if (TransformComponent.Position.y == (_WindowRegion.top + _WindowRegion.height) - (TransformComponent.Size.y)) {
					//	TransformComponent.Velocity.y -= _VERTICAL_IMPULSE * _GRAVITY_SCALE * DeltaTime;
					//}
				}
			}
		});
}
