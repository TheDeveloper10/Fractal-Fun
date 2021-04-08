#pragma once

#include "../../Core/Math/Vector2D.h"

#include "../FractalCore.h"
#include "../../Core/Drawing/Graphics/Graphics.h"

class TreeFractal : public FractalCore {
public:
	size_t MIN_BRANCH_LENGTH = 5;
	float ROTATION_ANGLE = 70.0f;
	float BRANCH_SCALE_DOWN_FACTOR = 0.7f;
private:
	Graphics* g = nullptr;
public:

	TreeFractal()
		: g(nullptr) { }

	void draw() override {
		Graphics temp = Graphics(m_image);
		g = &temp;

		branch(
			Point2D(m_image->getWidth() / 2, m_image->getHeight() - 10),
			Vector2D(0, 180), 3);

	}

	void branch(Point2D start, Vector2D vec, size_t thickness, int lbranchCount = 0, int rbranchCount = 0) {
		int x = start.x + (int)vec.x,
			y = start.y - (int)vec.y;

		if (x < 0)
			x = 0;
		if (y < 0)
			y = 0;
		Point2D end = Point2D(x, y);

		if (thickness < 1)
			thickness = 1;

		float len = vec.length() * 2.0f;
		Color col;

		col = Color(Color((int)(1.0f * len) % 255, (int)(2.0f * len) % 255, (int)(3.0f * len) % 255));
		//col = Color((int)(sinf(lbranchCount) * 122 + 123) % 255, (int)(cosf(rbranchCount) + 123) % 255, (int)(3.0f * len) % 255);
		//col = Color::fromHSV((int)((lbranchCount * 5 + rbranchCount * 10)) % 360, 100.0f, 100.0f);
		/*col = Color(
			((lbranchCount + 3) * 20) % 255, 
			((rbranchCount + 3) * 20) % 255, 
			((lbranchCount + rbranchCount + 3) * 10) % 255
		);*/
		//col = Color::fromHSV(k % 360, 100, 100);
		//k += 4;

		/*if(lbranchCount > rbranchCount)
			col = Color::fromHSV(((lbranchCount * 10) + k) % 360, 100, 100);
		else
			col = Color::fromHSV(360 - (((rbranchCount * 10) + k) % 360), 100, 100);*/

		g->drawLine(col, start, end, thickness);

		if (vec.length() >= MIN_BRANCH_LENGTH) {
			Vector2D lbranch_vec = Vector2D(vec.x, vec.y);
			lbranch_vec.rotate(ROTATION_ANGLE);
			branch(end, lbranch_vec * BRANCH_SCALE_DOWN_FACTOR, thickness, lbranchCount + 1, rbranchCount);// -1);

			Vector2D rbranch_vec = Vector2D(vec.x, vec.y);
			rbranch_vec.rotate(-ROTATION_ANGLE);
			branch(end, rbranch_vec * BRANCH_SCALE_DOWN_FACTOR, thickness, lbranchCount, rbranchCount + 1);// -1);
		}
	}

	void on_key_press(SDL_Event& e) {
		switch (e.key.keysym.sym) {
		case SDLK_1: {
			--ROTATION_ANGLE;
			break;
		}
		case SDLK_2: {
			++ROTATION_ANGLE;
			break;
		}
		case SDLK_3: {
			BRANCH_SCALE_DOWN_FACTOR -= 0.01f;
			break;
		}
		case SDLK_4: {
			BRANCH_SCALE_DOWN_FACTOR += 0.01f;
			break;
		}
		}
	}
};