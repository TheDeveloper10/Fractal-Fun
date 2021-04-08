#pragma once

#include "../../Core/Math/Vector2D.h"

#include "../FractalCore.h"
#include "../../Core/Drawing/Graphics/Graphics.h"

#include <vector>

class SierpinskisTriangle : public FractalCore {
public:
	float x_displ = 0,
		y_displ = 0;

	float x_displ_c = 0.1f, y_displ_c = 0.1f;
private:
	Graphics* g = nullptr;

	class Triangle {
	public:
		Vector2D a, b, c;

		Triangle(const Vector2D& a, const Vector2D& b, const Vector2D& c)
			: a(a), b(b), c(c) { }

		void draw(Graphics* g, const Vector2D& displ) {
			g->drawTriangle(
				Color(255, 255, 255),
				(a + displ).toPoint2D(),
				(b + displ).toPoint2D(),
				(c + displ).toPoint2D()
			);
		}

		Triangle* segment() const {
			Triangle* segments = (Triangle*)malloc(4 * sizeof(Triangle));

			Vector2D t1 = a.lerp(b, 0.5f);
			Vector2D t2 = a.lerp(c, 0.5f);
			Vector2D t3 = b.lerp(c, 0.5f);

			segments[0] = Triangle(a, t1, t2);
			segments[1] = Triangle(t1, b, t3);
			segments[2] = Triangle(t3, t2, c);

			return segments;
		}
	};

	std::vector<Triangle> trigs;

	void createPrimeTrig() {

		const float w_inner_factor = 0.15f,
			h_inner_factor = 0.15f,
			x_displ = 0.0f,
			y_displ = 0.0f;

		Triangle t = Triangle(
			Vector2D(m_image->getWidth() * w_inner_factor + x_displ, m_image->getHeight() * (1.0f - h_inner_factor) + y_displ),
			Vector2D(m_image->getWidth() * (1.0f - w_inner_factor) + x_displ, m_image->getHeight() * (1.0f - h_inner_factor) + y_displ),
			Vector2D(m_image->getWidth() / 2.0f + x_displ, m_image->getHeight() * h_inner_factor + y_displ)
		);

		trigs.push_back(t);
	}

	void segment() {
		std::vector<Triangle> newTrigs;

		for (size_t i = 0; i < trigs.size(); ++i) {
			Triangle* nt = trigs[i].segment();
			
			newTrigs.push_back(nt[0]);
			newTrigs.push_back(nt[1]);
			newTrigs.push_back(nt[2]);
		}

		trigs.clear();
		trigs = newTrigs;
	}
public:
	SierpinskisTriangle()
		: g(nullptr) { }

	void draw() override {
		if (trigs.size() <= 0) {
			createPrimeTrig();
		}

		Graphics temp = Graphics(m_image);
		g = &temp;

		Vector2D displ = Vector2D(x_displ, y_displ);
		for (size_t i = 0; i < trigs.size(); ++i)
			trigs[i].draw(g, displ);
	}

	void on_key_press(SDL_Event& e) {
		switch (e.key.keysym.sym) {
		case SDLK_1: {
			segment();
			break;
		}
		case SDLK_2: {
			trigs.clear();
			break;
		}
		case SDLK_LEFT: {
			x_displ += x_displ_c;
			break;
		}
		case SDLK_RIGHT: {
			x_displ -= x_displ_c;
			break;
		}
		case SDLK_UP: {
			y_displ += y_displ_c;
			break;
		}
		case SDLK_DOWN: {
			y_displ -= y_displ_c;
			break;
		}
		}
	}
};