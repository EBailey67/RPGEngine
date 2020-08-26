#pragma once

#include "../Utility/Vector2D.h"

void PlayerCreate(Vector2Di playerPos);
void SpriteRender();
void ParticleUpdate(const float dt);
void AnimationUpdate(const float dt);
void UIPanelsRender();
void UILabelsRender();
void LabelsRender();
void CreateLabels();
void ResetTable();
void ResetName();
