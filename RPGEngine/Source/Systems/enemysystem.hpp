#pragma once

#include "../component/colliders.hpp"
#include "../component/vector2d.hpp"

void EnemyCreate(Vector2D spawn);
void EnemyCharging(const CollisionData &lhs, const CollisionData &rhs);
void UpdateView();
void EnemyWalking(const float dt);
void HealthUpdate();
void Respawn();

