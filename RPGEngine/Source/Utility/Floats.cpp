#include "Floats.h"

#undef max
#undef min

namespace RPGEngine
{
    bool ApproxEqual(const float a, const float b, const float epsilon)
    {
        return std::abs(a - b) <= std::max(std::abs(a), std::abs(b)) * epsilon;
    }

    bool StrictlyLess(const float a, const float b, const float epsilon)
    {
        return (b - a) > std::max(std::abs(a), std::abs(b)) * epsilon;
    }

    bool ApproxEqual(const Vector2Df& a, const Vector2Df& b, const float epsilon)
    {
        return ApproxEqual(a.x, b.x, epsilon) &&
            ApproxEqual(a.y, b.y, epsilon);
    }

    bool StrictlyLess(Vector2Df& a, const Vector2Df& b, const float epsilon)
    {
        return StrictlyLess(a.x, b.x, epsilon) &&
            StrictlyLess(a.y, b.y, epsilon);
    }

    bool ApproxEqual(const Vector2Di& a, const Vector2Di& b)
    {
        return (a.x == b.x && a.y == b.y);
    }

    bool StrictlyLess(Vector2Di& a, const Vector2Di& b)
    {
        return (a.x < b.x && a.y < b.y);
    }

}

