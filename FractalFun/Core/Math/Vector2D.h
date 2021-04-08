#pragma once

#include <cmath>

#include "AdditionalMath.h"

struct Point2D;

struct Vector2D {
	float x, y;

	Vector2D() 
		: x(0), y (0) { }
	Vector2D(float x, float y) 
		: x(x), y(y) { }

	float length() const {
		return sqrtf(x * x + y * y);
	}
	
	float lengthSqr() const {
		return x * x + y * y;
	}

	void normalize() {
		float len = length();
		x /= len;
		y /= len;
	}

	float dot(const Vector2D& other) const {
		return x * other.x + y * other.y;
	}

	Vector2D lerp(const Vector2D& other, float t) const {
		return Vector2D(
			flerp(x, other.x, t), 
			flerp(y, other.y, t)
		);
	}

private:
	static float deg_sin(float s) {
		return sinf(s * M_PI / 180.0f);
	}
	static float deg_cos(float s) {
		return cosf(s * M_PI / 180.0f);
	}
	static float deg_acos(float s) {
		return acosf(s) * 180.0f / M_PI;
	}
	static float deg_asin(float s) {
		return asinf(s) * 180.0f / M_PI;
	}
public:

	// angle in deg
	// angle > 0 -> clockwise rotation
	// angle < 0 -> anti-clockwise rotation
	void rotate(float angle) {
		if (angle == 0.0001f) return;
		float curr_len = length();
		if (curr_len < 0.0001f) return;
		float curr_angle = deg_acos(x / curr_len); 

		if (y < 0)
			curr_angle = 360 - curr_angle;

		float new_angle = curr_angle - angle;
		
		x = deg_cos(new_angle) * curr_len;
		y = deg_sin(new_angle) * curr_len;
	}

	_NODISCARD Point2D toPoint2D() const {
		return Point2D(x, y);
	}

	Vector2D operator*(const Vector2D& rhs) const {
		return Vector2D(x * rhs.x, y * rhs.y);
	}

	Vector2D operator/(const Vector2D& rhs) const {
		return Vector2D(x / rhs.x, y / rhs.y);
	}

	Vector2D operator+(const Vector2D& rhs) const {
		return Vector2D(x + rhs.x, y + rhs.y);
	}

	Vector2D operator-(const Vector2D& rhs) const {
		return Vector2D(x - rhs.x, y - rhs.y);
	}

	Vector2D& operator+=(const Vector2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Vector2D& operator-=(const Vector2D& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}

	Vector2D& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		return *this;
	}

	Vector2D& operator/=(float scalar) {
		x /= scalar;
		y /= scalar;
		return *this;
	}

	Vector2D operator*(float scalar) const {
		return Vector2D(x * scalar, y * scalar);
	}

	Vector2D operator/(float scalar) const {
		return Vector2D(x / scalar, y / scalar);
	}
};