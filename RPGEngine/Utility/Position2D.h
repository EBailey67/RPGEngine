#pragma once
#include <cmath>

struct Position2D 
{
	int x, y;
};

constexpr Position2D operator+(const Position2D lhs, const Position2D rhs)
{
	return { lhs.x + rhs.x, lhs.y + rhs.y };
}

constexpr Position2D& operator+=(Position2D& lhs, const Position2D rhs)
{
	return lhs = lhs + rhs;
}

constexpr Position2D operator-(const Position2D lhs, const Position2D rhs)
{
	return { lhs.x - rhs.x, lhs.y - rhs.y };
}

constexpr Position2D operator*(const Position2D lhs, const int rhs) 
{
	return { lhs.x * rhs, lhs.y * rhs };
}

constexpr bool operator==(const Position2D lhs, const Position2D rhs) 
{
	return lhs.x == rhs.x && lhs.y == rhs.y;
}

constexpr bool operator!=(const Position2D lhs, const Position2D rhs) 
{
	return !(lhs == rhs);
}

inline int distance2(const Position2D a, const Position2D b) 
{
	const int dx = a.x - b.x;
	const int dy = a.y - b.y;
	return dx * dx + dy * dy;
}

inline float distance(const Position2D a, const Position2D b) 
{
	return static_cast<float>(std::sqrt(distance2(a, b)));
}