#pragma once

#include "../Component/components.hpp"
#include <SDL_rect.h>

void CollisionTileDetection(float dt);
void CollisionDetection();
void SetPlayerHealth(int hp);
void SetPlayerScore(int score);

void NullVelocity(const CollisionData &lhs, const CollisionData &rhs);
void OnHit(const CollisionData &lhs, const CollisionData &rhs);
std::pair<Vector2D, Vector2D> AABBW(const SDL_FRect &lhs, const SDL_FRect &rhs);
bool AABB(const SDL_FRect &lhs, const SDL_FRect &rhs);