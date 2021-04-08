#pragma once

#include <cstdint>
#include "../Math/AdditionalMath.h"

struct Color {
    uint8_t a,
            b,
            g,
            r;

	Color() 
		: r(0), g(0), b(0), a(0) { }

	Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
		: r(r), g(g), b(b), a(a) { }

public:

	static Color lerp(Color a, Color b, float p) {
		if (p > 1.0f)
			p = 1.0f;
		else if (p < 0.0f)
			p = 0.0f;

		return Color(
			flerp(a.r, b.r, p),
			flerp(a.g, b.g, p),
			flerp(a.b, b.b, p)
		);
	}

	static Color fromHSV(float H, float S, float V) {
        float s = S / 100;
        float v = V / 100;
        float C = s * v;
        float X = C * (1 - fabs(fmod(H / 60.0, 2) - 1));
        float m = v - C;
        float r, g, b;
        if (H >= 0 && H < 60)
            r = C, g = X, b = 0;
        else if (H >= 60 && H < 120)
            r = X, g = C, b = 0;
        else if (H >= 120 && H < 180)
            r = 0, g = C, b = X;
        else if (H >= 180 && H < 240)
            r = 0, g = X, b = C;
        else if (H >= 240 && H < 300)
            r = X, g = 0, b = C;
        else
            r = C, g = 0, b = X;

        return Color(
            (r + m) * 255, 
            (g + m) * 255, 
            (b + m) * 255
        );
	}
};