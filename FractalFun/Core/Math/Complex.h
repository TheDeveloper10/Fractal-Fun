#pragma once

#include "Vector2D.h"

struct Complex {
	double real, imaginary;

	Complex() 
		: real(0), imaginary(0) { }
	Complex(double real, double imaginary)
		: real(real), imaginary(imaginary) { }

	double length() const {
		return sqrtf(real * real + imaginary * imaginary);
	}

	double lengthSqr() const {
		return real * real + imaginary * imaginary;
	}

	// X -> real; Y -> imaginary
	Vector2D toVector() const {
		return Vector2D(real, imaginary);
	}

public:

	Complex operator*(const Complex& rhs) const {
		return Complex(real * rhs.real - imaginary * rhs.imaginary, real * rhs.imaginary + rhs.real * imaginary);
	}

	Complex operator+(const Complex& rhs) const {
		return Complex(real + rhs.real, imaginary + rhs.imaginary);
	}
};