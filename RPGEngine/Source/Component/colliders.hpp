#pragma once

#include <entt/entt.hpp>
#include <SDL_rect.h>
#include <array>
#include <map>
#include <string>

#include "vector2d.hpp"

struct RectCollider
{
    SDL_FRect rect;
    inline static bool hasDebugDraw = false;
};

struct TileGridCollider
{
};

struct Name
{
    std::string name;
};

struct CollisionData
{
    entt::entity id;
    SDL_FRect rect;
    std::pair<Vector2D, Vector2D> direction;
};

#ifndef LayersID
enum LayersID : std::size_t
{
    WALLS,
    PLAYER,
    ENEMY,
    FLOOR
};
#endif

struct CollisionLayer
{
    inline static bool Matrix[sizeof(LayersID)][sizeof(LayersID)]{{false, true, false, false},
                                                                  {true, false, true, false},
                                                                  {false, true, false, true},
                                                                  {false, false, false, false}};
    LayersID layer;
};

struct NullVelocityCollision
{
};