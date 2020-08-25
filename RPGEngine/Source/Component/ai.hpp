#pragma once

struct Pathfinding
{
    inline static constexpr auto DIAGONAL_COST = 14;
    inline static constexpr auto SIDE_COST = 10;
};

struct Active
{
};

struct View
{
    float multiplier;
};
