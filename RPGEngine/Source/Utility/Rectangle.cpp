#include "Rectangle.h"
#include <cstdlib>

namespace RPGEngine
{
    //-----------------------------------------------------------------------------
    // Purpose: Constructor
    //-----------------------------------------------------------------------------
    //template <typename T>
    //Rectangle<T>::Rectangle(T x, T y, T w, T h)
    //{
    //    this->x = x;
    //    this->y = y;
    //    this->w = w;
    //    this->h = h;
    //}

    //-----------------------------------------------------------------------------
    // Purpose: Check if rectangle contains a 2D vector
    //-----------------------------------------------------------------------------
    template <typename T>
    bool Rectangle<T>::Contains(Vector2D& vVec) const
    {
        if ((vVec.x >= x) &&
            (vVec.x <= x + w) &&
            (vVec.y >= y) &&
            (vVec.x <= y + h))
        {
            return true;
        }
        else
            return false;
    }

    //-----------------------------------------------------------------------------
    // Purpose: Check if rectangle contains a set of coords
    //-----------------------------------------------------------------------------
    template <typename T>
    bool Rectangle<T>::Contains(T x, T y) const
    {
        if ((x >= this->x) &&
            (x <= this->x + this->w) &&
            (y >= this->y) &&
            (x <= this->y + this->h))
        {
            return true;
        }

        return false;
    }

    //-----------------------------------------------------------------------------
    // Purpose: Return an empty rectangle
    //-----------------------------------------------------------------------------
    template <typename T>
    Rectangle<T> Rectangle<T>::Empty()
    {
        return Rectangle();
    }

    //-----------------------------------------------------------------------------
    // Purpose: Get intersection depth between two rectangles
    //-----------------------------------------------------------------------------
    template <typename T>
    Vector2D Rectangle<T>::GetIntersectionDepth(const Rectangle<T>& rectA, const Rectangle<T>& rectB)
    {
        // Calculate half sizes.
        float halfWidthA = rectA.w / 2.0f;
        float halfHeightA = rectA.h / 2.0f;
        float halfWidthB = rectB.w / 2.0f;
        float halfHeightB = rectB.h / 2.0f;

        // Calculate centers.
        const Vector2D centerA(rectA.x + halfWidthA, rectA.y + halfHeightA);
        const Vector2D centerB(rectB.x + halfWidthB, rectB.y + halfHeightB);

        // Calculate current and minimum-non-intersecting distances between centers.
        const auto distanceX = centerA.x - centerB.x;
        const auto distanceY = centerA.y - centerB.y;
        const auto minDistanceX = halfWidthA + halfWidthB;
        const auto minDistanceY = halfHeightA + halfHeightB;

        // If we are not intersecting at all, return (0, 0).
        if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
            return Vector2D::Zero();

        // Calculate and return intersection depths.
        const auto depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
        const auto depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
        return Vector2D(depthX, depthY);
    }

    //-----------------------------------------------------------------------------
    // Purpose: Gets the position of the center of the bottom edge of the rectangle.
    //-----------------------------------------------------------------------------
    template <typename T>
    Vector2D Rectangle<T>::GetBottomCenter(const Rectangle<T>& rect)
    {
        return Vector2D(static_cast<float>(rect.x + rect.w / 2.0f), static_cast<float>(rect.y + rect.h));
    }

    //-----------------------------------------------------------------------------
    // Purpose: Gets the position of the center point of a rectangle
    //-----------------------------------------------------------------------------
    template <typename T>
    Vector2D Rectangle<T>::GetCenter(const Rectangle<T>& rect)
    {
        return Vector2D(static_cast<float>(rect.x + rect.w / 2.0f), static_cast<float>(rect.y + rect.h / 2.0f));
    }

    //-----------------------------------------------------------------------------
    // Purpose: Gets the floating point distance between the center point
    //          of one rectangle and the center point of another.
    //-----------------------------------------------------------------------------
    template <typename T>
    float Rectangle<T>::GetDistance(const Rectangle<T>& rectA, const Rectangle<T>& rectB)
    {
        return Vector2D::Distance(GetCenter(rectA), GetCenter(rectB));
    }

    //-----------------------------------------------------------------------------
    // Purpose: Gets the unit vector from one rectangle to another
    //-----------------------------------------------------------------------------
    template <typename T>
    Vector2D Rectangle<T>::GetDirection(const Rectangle<T>& rectA, const Rectangle<T>& rectB)
    {
        Vector2D direction = GetCenter(rectA) - GetCenter(rectB);
        direction.Normalize();
        return direction;
    }

    template <typename T>
    Rectangle<T>& Rectangle<T>::operator= (const Rectangle<T>& r2)
    {
        if (this == &r2)
            return *this;

        x = r2.x;
        y = r2.y;
        w = r2.w;
        h = r2.h;

        return *this;
    }

    template <typename T>
    bool Rectangle<T>::operator== (const Rectangle<T>& r2) const
    {
        return ((w == r2.w) && (h == r2.h));
    }

    template <typename T>
    bool Rectangle<T>::operator!= (const Rectangle<T>& r2) const
    {
        return !((w == r2.w) && (h == r2.h));
    }

}