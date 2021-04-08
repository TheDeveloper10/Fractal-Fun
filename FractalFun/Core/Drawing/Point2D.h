#pragma once

#include <cstdint>

struct Point2D {
	int x, y;

	Point2D()
		: x(0), y(0) { }

	Point2D(int x, int y)
		: x(x), y(y) { }

	
	Point2D& operator+=(const Point2D& rhs) {
		x += rhs.x;
		y += rhs.y;
		return *this;
	}

	Point2D& operator-=(const Point2D& rhs) {
		x -= rhs.x;
		y -= rhs.y;
		return *this;
	}
};