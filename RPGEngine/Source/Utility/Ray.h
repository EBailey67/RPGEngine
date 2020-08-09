#pragma once

#include "Vector2D.h"
#include "Rectangle.h"

namespace RPGEngine
{

	class Boundary
	{
	public:
		Boundary() :
			bounds(0, 0, 0, 0)
		{}

		// This takes the two corner points
		Boundary(const int x1, const int y1, const int x2, const int y2) :
			bounds(x1, y1, x2 - x1, y2 - y1)
		{}

		Boundary(Rect& r) :
			bounds(r)
		{}

		Rect bounds;
	};

	class Ray
	{
	public:
		Ray() :
			pos(0, 0), dir(0, 0)
		{}

		Ray(const Ray& obj) = default;

		Ray(const Vector2D pos, const float angle) :
			pos(pos), dir(Vector2D::FromAngle(angle))
		{}

		Ray(const int x, const int y) :
			pos(static_cast<float>(x), static_cast<float>(y)), dir(1.0f, 0.0f)
		{}

		bool HitsWall(const Boundary& wall, Ray& rOut) const;

		Vector2D pos;
		Vector2D dir;
	};

}