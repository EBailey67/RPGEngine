#pragma once

#include "../Utility/Vector2D.h"

struct Pathfinding
{
    inline static constexpr auto DIAGONAL_COST = 14;
    inline static constexpr auto SIDE_COST = 10;
};

struct Enemy
{
    inline static int currentSpawn = 0;
    inline static constexpr auto MAX_SPAWNS = 8;
    inline static const Vector2D spawns[MAX_SPAWNS]
    {
        {250.0f, 250.0f},
        {500.0f, 500.0f},
        {100.0f, 400.09f},
        {974.667847f, 716.000488f},
        {600.0f, 140.0f},
        {1100.667603f, 271.333405f},
        {1135.333862f, 786.667297f},
        {1135.333862f, 200.0f},
    };
    bool isCharging = false;
    float time = 0.5;
    float dt = 0;
};

struct Health
{
    int health = 2;
};

struct Active
{
};

struct View
{
    float multiplier;
};
