#pragma once

#include <iostream>

class Vector2D
{
public:
	float x;
	float y;

	Vector2D();
	Vector2D(float x, float y);

	Vector2D& Add(const Vector2D vector);
	Vector2D& Subtract(const Vector2D vector);
	Vector2D& Multiply(const Vector2D vector);
	Vector2D& Divide(const Vector2D vector);

	friend Vector2D& operator+(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator-(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator*(Vector2D& v1, const Vector2D& v2);
	friend Vector2D& operator/(Vector2D& v1, const Vector2D& v2);

	Vector2D& operator+=(const Vector2D& vector);
	Vector2D& operator-=(const Vector2D& vector);
	Vector2D& operator*=(const Vector2D& vector);
	Vector2D& operator/=(const Vector2D& vector);

	Vector2D& operator*(const int& scale);
	Vector2D& Zero();

	friend std::ostream& operator<<(std::ostream& stream, const Vector2D& vector);

};