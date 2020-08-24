#pragma once

#include "Vector2D.h"

namespace RPGEngine
{
    template <typename T>
    class Rectangle
    {
    public:
        Rectangle(T x = 0, T y = 0, T w = 0, T h = 0) :
            x(x), y(y), w(w), h(h)
        {}
        ~Rectangle(void) {};

        inline T Left(void) const { return x; }
        inline T Right(void) const { return x + w; }
        inline T Top(void) const { return y; }
        inline T Bottom(void) const { return y + h; }

        bool Contains(Vector2D<T>& vVec) const;
        bool Contains(T x, T y) const;

        static Rectangle Empty();

        // Static methods below are derived from the RectangleExtensions class
        // written in C#, released under the MSPL
        static Vector2D<T> GetBottomCenter(const Rectangle<T>& rect);
        static float GetDistance(const Rectangle<T>& rectA, const Rectangle<T>& rectB);
        static Vector2D<T> GetDirection(const Rectangle<T>& rectA, const Rectangle<T>& rectB);

        Rectangle<T>& operator= (const Rectangle<T>& r2);

        bool operator== (const Rectangle<T>& r2) const;
        bool operator!= (const Rectangle<T>& r2) const;

        T x, y, w, h;
    };

    typedef Rectangle<int> Rect;
    typedef Rectangle<float> RectF;
    typedef Rectangle<double> RectD;

	template <typename T>
    Vector2D<T> GetIntersectionDepth(const Rectangle<T>& rectA, const Rectangle<T>& rectB);

	bool Intersect(const Rect& lhs, const Rect& rhs);
	Vector2Di GetCenter(const Rect& rect);
	//template <typename T>
	//Vector2D<T> GetCenter(const Rectangle<T>& rect);
}