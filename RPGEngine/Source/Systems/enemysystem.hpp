#pragma once

#include "../Component/colliders.hpp"
#include "../Utility/Vector2D.h"

void EnemyCreate(Vector2Df spawn);
void EnemyCharging(const CollisionData &lhs, const CollisionData &rhs);
void UpdateView();
void EnemyWalking(const float dt);
void HealthUpdate();
void Respawn();

