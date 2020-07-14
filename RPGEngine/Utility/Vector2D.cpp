#include "Vector2D.h"


Vector2D::Vector2D()
{
	x = 0.0f;
	y = 0.0f;
}

Vector2D::Vector2D(float x, float y)
{
	this->x = x;
	this->y = y;
}

Vector2D& Vector2D::Add(const Vector2D vector)
{
	this->x += vector.x;
	this->y += vector.y;

	return *this;
}


Vector2D& Vector2D::Subtract(const Vector2D vector)
{
	this->x -= vector.x;
	this->y -= vector.y;

	return *this;
}


Vector2D& Vector2D::Multiply(const Vector2D vector)
{
	this->x *= vector.x;
	this->y *= vector.y;

	return *this;
}


Vector2D& Vector2D::Divide(const Vector2D vector)
{
	this->x /= vector.x;
	this->y /= vector.y;

	return *this;
}


Vector2D& operator+(Vector2D& v1, const Vector2D& v2)
{
	return v1.Add(v2);
}

Vector2D& operator-(Vector2D& v1, const Vector2D& v2)
{
	return v1.Subtract(v2);
}

Vector2D& operator*(Vector2D& v1, const Vector2D& v2)
{
	return v1.Multiply(v2);
}

Vector2D& operator/(Vector2D& v1, const Vector2D& v2)
{
	return v1.Divide(v2);
}

Vector2D& Vector2D::operator+=(const Vector2D& vector)
{
	return this->Add(vector);
}

Vector2D& Vector2D::operator-=(const Vector2D& vector)
{
	return this->Subtract(vector);
}

Vector2D& Vector2D::operator*=(const Vector2D& vector)
{
	return this->Multiply(vector);
}

Vector2D& Vector2D::operator/=(const Vector2D& vector)
{
	return this->Divide(vector);
}

Vector2D& Vector2D::operator*(const int& scale)
{
	this->x *= scale;
	this->y *= scale;
	return *this;
}

Vector2D& Vector2D::Zero()
{
	this->x = 0.0f;
	this->y = 0.0f;
	return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vector2D& vector)
{
	stream << "(" << vector.x << "," << vector.y << ")";
	return stream;
}
