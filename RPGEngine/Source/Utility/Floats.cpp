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

    bool ApproxEqual(const Vector2D& a, const Vector2D& b, const float epsilon)
    {
        return ApproxEqual(a.x, b.x, epsilon) &&
            ApproxEqual(a.y, b.y, epsilon);
    }

    bool StrictlyLess(Vector2D& a, const Vector2D& b, const float epsilon)
    {
        return StrictlyLess(a.x, b.x, epsilon) &&
            StrictlyLess(a.y, b.y, epsilon);
    }
}

