#pragma once

#include <limits>
#include <algorithm>

#include "Vector2D.h"


namespace RPGEngine
{
    bool ApproxEqual(const float a, const float b, const float epsilon = std::numeric_limits<float>::epsilon());
    inline bool StrictlyLess(const float a, const float b, const float epsilon = std::numeric_limits<float>::epsilon());
    bool ApproxEqual(const Vector2Df& a, const Vector2Df& b, const float epsilon = std::numeric_limits<float>::epsilon());
    bool StrictlyLess(Vector2Df& a, const Vector2Df& b, const float epsilon = std::numeric_limits<float>::epsilon());
    bool ApproxEqual(const Vector2Di& a, const Vector2Di& b);
    bool StrictlyLess(Vector2Di& a, const Vector2Di& b);


}

