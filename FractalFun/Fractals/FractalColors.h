#pragma once

#include "../Core/Math/Complex.h"
#include "../Core/Math/AdditionalMath.h"
#include "../Core/Drawing/Color.h"

inline Color color_based_on_iteration(int iteration, const int max_iterations) {
	// First way to color
	//if (iteration < max_iterations) {
	//	int k = ((float)iteration / max_iterations) * 255;
	//	return Color(
	//		k*k*k,
	//		0,
	//		0);
	//	//return Color(
	//	//	0,
	//	//	k,
	//	//	0);
	//}
	//return Color(0, 0, 0);

	// Second way to color
	return Color::fromHSV(360.0f * iteration / max_iterations, 100.0f, iteration < max_iterations ? 100 : 0);
}