#pragma once

#include "../Component/Map.h"
#include "../Terminal/sdl_context.hpp"

void MapCreate(int w, int h);
void MapRender(Term::SDL::Context& context);
void CreateMap(int w, int h);
//void LightsRender();
//void FOVCalculate();
//void UpdateVisibility();
