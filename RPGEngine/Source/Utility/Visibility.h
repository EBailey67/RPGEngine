#pragma once

#include <set>
#include <vector>
#include <algorithm>
#include <limits>
#include <cmath>
#include <cassert>

#include "floats.h"
#include "primitives.h"

namespace RPGEngine
{
    /* Compare 2 line segments based on their distance from given point
     * Assumes: (1) the line segments are intersected by some ray from the origin
     *          (2) the line segments do not intersect except at their endpoints
     *          (3) no line segment is collinear with the origin
     */
    template<typename Vector>
    struct line_segment_dist_comparer
    {
        using segment_type = LineSegment;

        Vector origin;

        explicit line_segment_dist_comparer(Vector origin) :
            origin(origin)
        {
        }

        /** Check whether the line segment x is closer to the origin than the
         * line segment y.
         * @param x line segment: left hand side of the comparison operator
         * @param y line segment: right hand side of the comparison operator
         * @return true iff x < y (x is closer than y)
         */
        bool operator()(const segment_type& x, const segment_type& y) const
        {
            auto a = x.a, b = x.b;
            auto c = y.a, d = y.b;

            assert(
                ComputeOrientation(origin, a, b) != Orientation::collinear &&
                "AB must not be collinear with the origin.");
            assert(
                ComputeOrientation(origin, c, d) != Orientation::collinear &&
                "CD must not be collinear with the origin.");

            // sort the endpoints so that if there are common endpoints,
            // it will be a and c 
            if (ApproxEqual(b, c) || ApproxEqual(b, d))
                std::swap(a, b);
            if (ApproxEqual(a, d))
                std::swap(c, d);

            // cases with common endpoints
            if (ApproxEqual(a, c))
            {
                auto oad = ComputeOrientation(origin, a, d);
                auto oab = ComputeOrientation(origin, a, b);
                if (ApproxEqual(b, d) || oad != oab)
                    return false;
                return ComputeOrientation(a, b, d) != ComputeOrientation(a, b, origin);
            }

            // cases without common endpoints
            auto cda = ComputeOrientation(c, d, a);
            auto cdb = ComputeOrientation(c, d, b);
            if (cdb == Orientation::collinear && cda == Orientation::collinear)
            {
                return Vector2D<Vector>::DistanceSquared(origin, a) < Vector2D<Vector>::DistanceSquared(origin, c);
            }
            else if (cda == cdb ||
                cda == Orientation::collinear ||
                cdb == Orientation::collinear)
            {
                auto cdo = ComputeOrientation(c, d, origin);
                return cdo == cda || cdo == cdb;
            }
            else
            {
                auto abo = ComputeOrientation(a, b, origin);
                return abo != ComputeOrientation(a, b, c);
            }
        }
    };

    // compare angles clockwise starting at the positive y axis
    template<typename Vector>
    struct angle_comparer
    {
        Vector vertex;

        explicit angle_comparer(Vector origin) : vertex(origin) {}

        bool operator()(const Vector& a, const Vector& b) const
        {
            auto is_a_left = StrictlyLess(a.x, vertex.x);
            auto is_b_left = StrictlyLess(b.x, vertex.x);
            if (is_a_left != is_b_left)
                return is_b_left;

            if (ApproxEqual(a.x, vertex.x) && ApproxEqual(b.x, vertex.x))
            {
                if (!StrictlyLess(a.y, vertex.y) ||
                    !StrictlyLess(b.y, vertex.y))
                {
                    return StrictlyLess(b.y, a.y);
                }
                return StrictlyLess(a.y, b.y);
            }

            auto oa = a - vertex;
            auto ob = b - vertex;
            auto det = Vector2D<Vector>::Cross(oa, ob);
            if (ApproxEqual(det, 0.f))
            {
                return Vector2D<Vector>::Dot(oa, oa) < Vector2D<Vector>::Dot(ob, ob);
            }
            return det < 0;
        }
    };

    template<typename Vector>
    struct VisibilityEvent
    {
        // events used in the visibility polygon algorithm
        enum EventType
        {
            start_vertex,
            end_vertex
        };

        EventType type;
        LineSegment segment;

        VisibilityEvent() {}
        VisibilityEvent(EventType type, const LineSegment& segment) :
            type(type),
            segment(segment) {}

        [[nodiscard]] const auto& point() const { return segment.a; }
    };

    /** Calculate visibility polygon vertices in clockwise order.
     * Endpoints of the line segments (obstacles) can be ordered arbitrarily.
     * Line segments collinear with the point are ignored.
     * @param point - position of the observer
     * @param begin iterator of the list of line segments (obstacles)
     * @param end iterator of the list of line segments (obstacles)
     * @return vector of vertices of the visibility polygon
     */
    template<typename Vector, typename InputIterator>
    std::vector<Vector> VisibilityPolygon(
        Vector point,
        InputIterator begin,
        InputIterator end)
    {
        using segment_type = LineSegment;
        using event_type = VisibilityEvent<Vector>;
        using segment_comparer_type = line_segment_dist_comparer<Vector>;

        segment_comparer_type cmp_dist{ point };
        std::set<segment_type, segment_comparer_type> state{ cmp_dist };
        std::vector<event_type> events;

        for (; begin != end; ++begin)
        {
            auto segment = *begin;

            // Sort line segment endpoints and add them as events
            // Skip line segments collinear with the point
            auto pab = ComputeOrientation(point, segment.a, segment.b);
            if (pab == Orientation::collinear)
            {
                continue;
            }
            else if (pab == Orientation::right_turn)
            {
                events.emplace_back(event_type::start_vertex, segment);
                events.emplace_back(
                    event_type::end_vertex,
                    segment_type{ segment.b, segment.a });
            }
            else
            {
                events.emplace_back(
                    event_type::start_vertex,
                    segment_type{ segment.b, segment.a });
                events.emplace_back(event_type::end_vertex, segment);
            }

            // Initialize state by adding line segments that are intersected
            // by vertical ray from the point
            auto a = segment.a, b = segment.b;
            if (a.x > b.x)
                std::swap(a, b);

            auto abp = ComputeOrientation(a, b, point);
            if (abp == Orientation::right_turn &&
                (ApproxEqual(b.x, point.x) ||
                    (a.x < point.x && point.x < b.x)))
            {
                state.insert(segment);
            }
        }

        // sort events by angle
        angle_comparer<Vector> cmp_angle{ point };
        std::sort(events.begin(), events.end(), [&cmp_angle](auto&& a, auto&& b)
            {
                // if the points are equal, sort end vertices first
                if (ApproxEqual(a.point(), b.point()))
                    return a.type == event_type::end_vertex &&
                    b.type == event_type::start_vertex;
                return cmp_angle(a.point(), b.point());
            });

        // find the visibility polygon
        std::vector<Vector> vertices;
        for (auto&& event : events)
        {
            if (event.type == event_type::end_vertex)
                state.erase(event.segment);

            if (state.empty())
            {
                vertices.push_back(event.point());
            }
            else if (cmp_dist(event.segment, *state.begin()))
            {
                // Nearest line segment has changed
                // Compute the intersection point with this segment
                Vector2D intersection;
                Ray<Vector> ray{ point, event.point() - point };
                auto nearest_segment = *state.begin();
                auto intersects = ray.Intersects(nearest_segment, intersection);
            	if (!intersects)
            	{
					assert(intersects && "Ray intersects line segment L iff L is in the state");
                }

                if (event.type == event_type::start_vertex)
                {
                    vertices.push_back(intersection);
                    vertices.push_back(event.point());
                }
                else
                {
                    vertices.push_back(event.point());
                    vertices.push_back(intersection);
                }
            }

            if (event.type == event_type::start_vertex)
                state.insert(event.segment);
        }

        // remove collinear points
        auto top = vertices.begin();
        for (auto it = vertices.begin(); it != vertices.end(); ++it)
        {
            auto prev = top == vertices.begin() ? vertices.end() - 1 : top - 1;
            auto next = it + 1 == vertices.end() ? vertices.begin() : it + 1;
            if (ComputeOrientation(*prev, *it, *next) != Orientation::collinear)
                *top++ = *it;
        }
        vertices.erase(top, vertices.end());
        // if we have a polygon, then make sure the last point is the same
    	// as the first to close it properly.
    	if (vertices.size() > 2)
    		vertices.emplace_back(vertices[0]);
        return vertices;
    }
}

