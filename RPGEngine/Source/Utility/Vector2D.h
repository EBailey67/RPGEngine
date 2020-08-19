#pragma once

#include <cmath>
#include <SDL_rect.h>
#include <string>


/**
 * The Vector2D class.
 * Representation of 2D vectors and points.
 */
template <class T>
class Vector2D
{

public:
    T x{};
    T y{};

    // Shorthand for writing Vector2(0, 0).
    static constexpr auto Zero()
    {
        return Vector2D<T>();
    }

    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr auto Up()
    {
        return Vector2D<T>(0, 1);
    }

    /*! @brief Shorthand for writing Vector2(0, -1). */
    static constexpr auto Down()
    {
        return Vector2D<T>(0, -1);
    }

    /*! @brief Shorthand for writing Vector2(-1, 0). */
    static constexpr auto Left()
    {
        return Vector2D<T>(-1, 0);
    }

    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr auto Right()
    {
        return Vector2D<T>(0, 1);
    }

    /*! @brief Shorthand for writing Vector2(1, 1). */
    static constexpr auto One()
    {
        return Vector2D<T>(1, 1);
    }

	/*! @brief Returns a unit vector based on the angle (in radians) */
	static constexpr auto FromAngle(const float radians)
    {
		return Vector2D<T>(cosf(radians), sinf(radians));
    }
	
    /*! @brief Default constructor !*/
    constexpr Vector2D() : x(0), y(0)
    {
    }

    /*! @brief Construct vector with desired coordinates. */
    constexpr Vector2D(const float x, const float y) : x(static_cast<T>(x)), y(static_cast<T>(y))
    {
    }

	/*! @brief Construct vector with desired coordinates. */
    constexpr Vector2D(const int x, const int y) : x(static_cast<T>(x)), y(static_cast<T>(y))
    {
    }

    /*! @brief Copy constructor. */
    Vector2D(const Vector2D<T> &other) = default;

    /*! @brief Default destructor. */
    ~Vector2D() = default;

    /*! @brief Compute vector length and returns it. */
    [[nodiscard]] float Magnitude() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    /*! @brief Returns vector with length 1. */
    [[nodiscard]] Vector2D Normalized() const noexcept
    {
	    const auto length = Magnitude();
        return Vector2D<T>(static_cast<T>(x / length), static_cast<T>(y / length));
    }

    /*! @brief Self normalize. */
    void Normalize()
    {
        *this = Normalized();
    }

    /*! @brief Set x and y coordinates. */
    void Set(const T xIn, const T yIn) noexcept
    {
        x = xIn;
        y = yIn;
    }

    /*! @brief Check if two vectors are equals. */
    static bool Equals(const Vector2D<T> &lhs, const Vector2D<T> &rhs) noexcept
    {
        return lhs == rhs;
    }

    /*! @brief returns the cross-product of two vectors */
	static T Cross(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
    {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    /*! @brief returns the dot-product of two vectors */
    static T Dot(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
    {
        return lhs.x * rhs.x - lhs.y * rhs.y;
    }

    /*! @brief returns the distance between two vectors */
    static float Distance(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
    {
	    const auto xd = rhs.x - lhs.x;
	    const auto yd = rhs.y - lhs.y;
        return sqrtf(xd * xd + yd * yd);
    }

	/*! @brief returns the squared distance between two vectors */
    static float DistanceSquared(const Vector2D<T>& lhs, const Vector2D<T>& rhs)
    {
	    const auto xd = rhs.x - lhs.x;
	    const auto yd = rhs.y - lhs.y;
        return xd * xd + yd * yd;
    }

    /*! @brief Returns formatted coordinates in string. */
    [[nodiscard]] std::string ToString() const noexcept
    {
        return "x: " + std::to_string(x) + "\t" + "y:" + std::to_string(y);
    }

    /*! @brief Copy assignment. */
    Vector2D<T> &operator=(const Vector2D<T> &other)
    {
        if (this != &other)
        {
            this->x = other.x;
            this->y = other.y;
        }

        return *this;
    }
	
    [[nodiscard]] Vector2D<int> ToInt() const noexcept
    {
	    return Vector2D<int>(x, y);
    }

    [[nodiscard]] Vector2D<float> ToFloat() const noexcept
    {
	    return Vector2D<float>(x, y);
    }

	
    /*! @brief Multiplies vector by number. */
    Vector2D<T> operator*(const float multiplier) const noexcept
    {
        Vector2D<T> other(x * multiplier, y * multiplier);
        return other;
    }
	
    /*! @brief Divides vector by number. */
    Vector2D<T> operator/(const float divider) const
    {
        Vector2D<T> other(x / divider, y / divider);
        return other;
    }

    /*! @brief Adds to vectors. */
    Vector2D<T> operator+(const Vector2D<T> &other) const noexcept
    {
        Vector2D<T> result(x + other.x, y + other.y);
        return result;
    }

    /*! @brief Subtracts one vector from another. */
    Vector2D<T> operator-(const Vector2D<T> &other) const noexcept
    {
        Vector2D<T> result(x - other.x, y - other.y);
        return result;
    }

    /*! @brief Returns true if two vectors are approximately equal. */
    bool operator==(const Vector2D<T> &other) const noexcept
    {
        return (x == other.x && y == other.y);
    }

    /*! @brief Returns false if two vectors are approximately equal. */
    bool operator!=(const Vector2D<T> &other) const noexcept
    {
        return !(*this == other);
    }

	/**
     * \brief Linear Interpolation between two 2D Vectors
     * \param a  - start (t = 0)
     * \param b  - end (t = 1)
     * \param t  = between 0 and 1 
     * \return a vector between a and b specified as time 't'
     */
    static Vector2D<T> Lerp(const Vector2D<T> &a, const Vector2D<T> &b, const float t)
    {
        return b * t + a * (1 - t);
    }

	[[nodiscard]] bool InRect(const SDL_Rect rect) const
	{
	    if (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h)
        return true;

    	return false;
    }
};

typedef Vector2D<float> Vector2Df;
typedef Vector2D<int> Vector2Di;
