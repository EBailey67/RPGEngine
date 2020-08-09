#pragma once

#include <limits>
#include <algorithm>

#include "Vector2D.h"


namespace RPGEngine
{
    bool ApproxEqual(const float a, const float b, const float epsilon = std::numeric_limits<float>::epsilon());
    inline bool StrictlyLess(const float a, const float b, const float epsilon = std::numeric_limits<float>::epsilon());
    bool ApproxEqual(const Vector2D a, const Vector2D b, const float epsilon = std::numeric_limits<float>::epsilon());
    bool StrictlyLess(Vector2D& a, const Vector2D b, const float epsilon = std::numeric_limits<float>::epsilon());
}

