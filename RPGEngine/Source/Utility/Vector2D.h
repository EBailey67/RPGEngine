#pragma once

#include <cmath>
#include <SDL_rect.h>
#include <string>


/**
 * The Vector2D class.
 * Representation of 2D vectors and points.
 */
class Vector2D
{

public:
    float x{};
    float y{};

    // Shorthand for writing Vector2(0, 0).
    static constexpr auto Zero()
    {
        return Vector2D();
    }

    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr auto Up()
    {
        return Vector2D(0, 1);
    }

    /*! @brief Shorthand for writing Vector2(0, -1). */
    static constexpr auto Down()
    {
        return Vector2D(0, -1);
    }

    /*! @brief Shorthand for writing Vector2(-1, 0). */
    static constexpr auto Left()
    {
        return Vector2D(-1, 0);
    }

    /*! @brief Shorthand for writing Vector2(0, 1). */
    static constexpr auto Right()
    {
        return Vector2D(0, 1);
    }

    /*! @brief Shorthand for writing Vector2(1, 1). */
    static constexpr auto One()
    {
        return Vector2D(1, 1);
    }

	/*! @brief Returns a unit vector based on the angle (in radians) */
	static constexpr auto FromAngle(const float radians)
    {
		return Vector2D(cosf(radians), sinf(radians));
    }
	
    /*! @brief Default constructor !*/
    constexpr Vector2D() : x(0), y(0)
    {
    }

    /*! @brief Construct vector with desired coordinates. */
    constexpr Vector2D(const float x, const float y) : x(x), y(y)
    {
    }

	/*! @brief Construct vector with desired coordinates. */
    constexpr Vector2D(const int x, const int y) : x(static_cast<float>(x)), y(static_cast<float>(y))
    {
    }

    /*! @brief Copy constructor. */
    Vector2D(const Vector2D &other) = default;

    /*! @brief Default destructor. */
    ~Vector2D() = default;

    /*! @brief Compute vector length and returns it. */
    [[nodiscard]] float Magnitude() const noexcept
    {
        return std::sqrt(x * x + y * y);
    }

    /*! @brief Returns vector with lenght 1. */
    [[nodiscard]] Vector2D Normalized() const noexcept
    {
	    const auto length = Magnitude();
        return Vector2D(x / length, y / length);
    }

    /*! @brief Self normalize. */
    void Normalize()
    {
        *this = Normalized();
    }

    /*! @brief Set x and y coordinates. */
    void Set(const float xIn, const float yIn) noexcept
    {
        x = xIn;
        y = yIn;
    }

    /*! @brief Check if two vectors are equals. */
    static bool Equals(const Vector2D &lhs, const Vector2D &rhs) noexcept
    {
        return lhs == rhs;
    }

    /*! @brief returns the cross-product of two vectors */
	static float Cross(const Vector2D& lhs, const Vector2D& rhs)
    {
        return lhs.x * rhs.y - lhs.y * rhs.x;
    }

    /*! @brief returns the dot-product of two vectors */
    static float Dot(const Vector2D& lhs, const Vector2D& rhs)
    {
        return lhs.x * rhs.x - lhs.y * rhs.y;
    }

    /*! @brief returns the distance between two vectors */
    static float Distance(const Vector2D& lhs, const Vector2D& rhs)
    {
	    const auto xd = rhs.x - lhs.x;
	    const auto yd = rhs.y - lhs.y;
        return sqrtf(xd * xd + yd * yd);
    }

	/*! @brief returns the squared distance between two vectors */
    static float DistanceSquared(const Vector2D& lhs, const Vector2D& rhs)
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
    Vector2D &operator=(const Vector2D &other)
    {
        if (this != &other)
        {
            this->x = other.x;
            this->y = other.y;
        }

        return *this;
    }
	
    /*! @brief Multiplies vector by number. */
    Vector2D operator*(const float multiplier) const noexcept
    {
        Vector2D other(x * multiplier, y * multiplier);
        return other;
    }
	
    /*! @brief Divides vector by number. */
    Vector2D operator/(const float divider) const
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

	/**
     * \brief Linear Interpolation between two 2D Vectors
     * \param a  - start (t = 0)
     * \param b  - end (t = 1)
     * \param t  = between 0 and 1 
     * \return a vector between a and b specified as time 't'
     */
    static Vector2D Lerp(const Vector2D &a, const Vector2D &b, const float t)
    {
        return b * t + a * (1 - t);
    }

	bool InRect(SDL_Rect rect) const
	{
	    if (x >= rect.x && x <= rect.x + rect.w &&
            y >= rect.y && y <= rect.y + rect.h)
        return true;

    	return false;
    }
};
