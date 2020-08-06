#pragma once

#include <cmath>
#include <string>


/**
 * The Vector2D class.
 * Representation of 2D vectors and points.
 */
class Vector2D
{

public:
    using coordinate_type = float;

    coordinate_type x;
    coordinate_type y;


    // Shorthand for writing Vector2(0, 0).
    static constexpr const auto zero()
    {
        return Vector2D();
    }

    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr const auto up()
    {
        return Vector2D(0, 1);
    }

    /*! @brief Shorthand for writing Vector2(0, -1). */
    static constexpr const auto down()
    {
        return Vector2D(0, -1);
    }

    /*! @brief Shorthand for writing Vector2(-1, 0). */
    static constexpr const auto left()
    {
        return Vector2D(-1, 0);
    }

    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr const auto right()
    {
        return Vector2D(0, 1);
    }

    /*! @brief Shorthand for writing Vector2(1, 1). */
    static constexpr const auto one()
    {
        return Vector2D(1, 1);
    }

    /*! @brief Default constructor !*/
    constexpr Vector2D() : x(0), y(0)
    {
    }

    /*! @brief Construct vector with desired coordinates. */
    constexpr Vector2D(const coordinate_type x, const coordinate_type y) : x(x), y(y)
    {
    }
    /*! @brief Copy constructor. */
    Vector2D(const Vector2D &other)
    {
        *this = other;
    }
    /*! @brief Default destructor. */
    ~Vector2D() = default;

    ///*! @brief Returns x coordinate. */
    //const coordinate_type &x() const noexcept
    //{
    //    return x;
    //}

    ///*! @brief Returns y coordinate. */
    //const coordinate_type &y() const noexcept
    //{
    //    return y;
    //}

    /*! @brief Compute vector lenght and returns it. */
    const coordinate_type magnitude() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    /*! @brief Returns vector with lenght 1. */
    const Vector2D normalized() const noexcept
    {
        auto length = magnitude();
        return Vector2D(x / length, y / length);
    }

    /*! @brief Self normalize. */
    void Normalize()
    {
        *this = normalized();
    }

    /*! @brief Set x and y coordinates. */
    void Set(const coordinate_type xIn, const coordinate_type yIn) noexcept
    {
        x = xIn;
        y = yIn;
    }

    /*! @brief Check if two vectors are equals. */
    bool Equals(const Vector2D &lhs, const Vector2D &rhs) const noexcept
    {
        return lhs == rhs;
    }

    /*! @brief Returns formated coordinates in string. */
    std::string ToString() const noexcept
    {
        return "x: " + std::to_string(x) + "\t" + "y:" + std::to_string(y);
    }

    /*! @brief Copy assgment. */
    Vector2D &operator=(const Vector2D &other)
    {
        if (this != &other)
        {
            this->x = other.x;
            this->y = other.y;
        }

        return *this;
    }
    /*! @brief Multiplys vector by number. */
    Vector2D operator*(coordinate_type multiplier) const noexcept
    {
        Vector2D other(x * multiplier, y * multiplier);
        return other;
    }
    /*! @brief Divides vector by number. */
    Vector2D operator/(coordinate_type divider) const
    {
        Vector2D other(x / divider, y / divider);
        return other;
    }

    /*! @brief Adds to vectors. */
    Vector2D operator+(const Vector2D &other) const noexcept
    {
        Vector2D result(x + other.x, y + other.y);
        return result;
    }

    /*! @brief Subtracts one vector from another. */
    Vector2D operator-(const Vector2D &other) const noexcept
    {
        Vector2D result(x - other.x, y - other.y);
        return result;
    }

    /*! @brief Returns true if two vectors are approximately equal. */
    bool operator==(const Vector2D &other) const noexcept
    {
        return (x == other.x && y == other.y);
    }

    /*! @brief Returns false if two vectors are approximately equal. */
    bool operator!=(const Vector2D &other) const noexcept
    {
        return !(*this == other);
    }

    static Vector2D Lerp(const Vector2D &a, const Vector2D &b, float t)
    {
        return b * t + a * (1 - t);
    }
};
