#pragma once

#include <limits>
#include <cmath>

#include "Floats.h"

namespace RPGEngine
{
    enum Orientation
    {
        left_turn = 1,
        right_turn = -1,
        collinear = 0
    };

    /** Compute orientation of 3 points in a plane.
     * @param a first point
     * @param b second point
     * @param c third point
     * @return orientation of the points in the plane (left turn, right turn
     *         or collinear)
     */
    inline Orientation ComputeOrientation(const Vector2Df a, const Vector2Df b, const Vector2Df c)
    {
	    const auto det = Vector2Df::Cross(b - a, c - a);
        return static_cast<Orientation>(
            static_cast<int>(StrictlyLess(0.f, det)) -
            static_cast<int>(StrictlyLess(det, 0.f))
            );
    }

    inline Orientation ComputeOrientation(const Vector2Di a, const Vector2Di b, const Vector2Di c)
    {
	    const auto det = Vector2Di::Cross(b - a, c - a);
        return static_cast<Orientation>(
            static_cast<int>(StrictlyLess(0.f, static_cast<float>(det))) -
            static_cast<int>(StrictlyLess(static_cast<float>(det), 0.f))
            );
    }

	
    struct Light
    {
	    Vector2Df position;
    	SDL_Color color;
    	float energy;
    	float radius;
    };

	struct Cell
	{
		int edge_id[4]{};
		bool edge_exist[4]{};
		bool exist = false;
	};
	
    struct LineSegment
    {
        Vector2Di a, b;

        LineSegment() = default;
        LineSegment(const Vector2Df a, const Vector2Df b) : a(a.ToInt()), b(b.ToInt()) {}
        LineSegment(const Vector2Di a, const Vector2Di b) : a(a), b(b) {}
        LineSegment(const LineSegment&) = default;
        LineSegment& operator=(const LineSegment& segment) = default;
    };

    template<typename Vector>
    struct Ray
    {
        Vector origin;
        Vector direction;

        Ray() {}
        Ray(Vector origin, Vector direction) :
            origin(origin),
            direction(direction) {}

        /** Find the nearest intersection point of ray and line segment.
         * @param segment
         * @param out_point reference to a variable where the nearest
         *        intersection point will be stored (can be changed even
         *        when there is no intersection)
         * @return true iff the ray intersects the line segment
         */
        bool Intersects(const LineSegment& segment, Vector2D<Vector>& out_point) const
        {
            auto ao = origin - segment.a;
            auto ab = segment.b - segment.a;
            auto det = Vector2D<Vector>::Cross(ab, direction);
            if (ApproxEqual(det, 0.f))
            {
                auto abo = ComputeOrientation(segment.a, segment.b, origin);
                if (abo != Orientation::collinear)
                    return false;
                auto dist_a = Vector2D<Vector>::Dot(ao, direction);
                auto dist_b = Vector2D<Vector>::Dot(origin - segment.b, direction);

                if (dist_a > 0 && dist_b > 0)
                    return false;
                else if ((dist_a > 0) != (dist_b > 0))
                    out_point = origin;
                else if (dist_a > dist_b)  // at this point, both distances are negative
                    out_point = segment.a; // hence the nearest point is A
                else
                    out_point = segment.b;
                return true;
            }
            
            auto u = Vector2D<Vector>::Cross(ao, direction) / det;
            if (StrictlyLess(u, 0.f) ||
                StrictlyLess(1.f, u))
                return false;

            float t = -Vector2D<Vector>::Cross(ab, ao) / det;
            out_point = origin + direction * t;
            return ApproxEqual(t, 0.f) || t > 0;
        }
    };
}

