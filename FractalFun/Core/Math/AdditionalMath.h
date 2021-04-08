#pragma once

inline float map(float value, float curr_min, float curr_max, float new_min, float new_max) {
	return new_min + ((value - curr_min) / (curr_max - curr_min)) * (new_max - new_min);
}

static inline float flerp(float a, float b, float t) {
	return a + t * (b - a);
}