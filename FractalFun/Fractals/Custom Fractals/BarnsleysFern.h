#pragma once

#include "../../Core/Math/AdditionalMath.h"
#include "../../Core/Drawing/Point2D.h"
#include "../FractalCore.h"

class BarnsleysFern : public FractalCore {
public:
	int64_t iterations_count = 100000;
	
	float w_min_map = -2.182f, w_max_map = 2.6558f,
		  h_min_map = 0, h_max_map = 9.9983f,
		  move_factor = 0.02f, zoom_factor = 0.04f, zoomed_down_factor = 0.98f;
public:
	float x, newX, 
		  y, newY,
		  r;
	
	void move_coords() {
		r = (float)rand() / RAND_MAX;

		if (r < 0.01f) {
			newX = 0;
			newY = 0.16f * y;
		}
		else if (r < 0.86f) {
			newX = 0.85f * x + 0.04f * y;
			newY = -0.04f * x + 0.85f * y + 1.6f;
		}
		else if (r < 0.93f) {
			newX = 0.2f * x + -0.26f * y;
			newY = 0.23f * x + 0.22f * y + 1.6f;
		}
		else {
			newX = -0.15f * x + 0.28f * y;
			newY = 0.26f * x + 0.24f * y + 0.44f;
		}

		x = newX;
		y = newY;
	}

	void draw_coords() {
		size_t x_mapped = map(x, w_min_map, w_max_map, 0, m_image->getWidth());
		size_t y_mapped = map(y, h_min_map, h_max_map, m_image->getHeight(), 0);
		m_image->setPixel(
							Point2D(x_mapped, y_mapped), 
							Color(0, 255, 0)
						);
	}
private:
	void draw() override {
		// same seed so it can generate same numbers every time
		srand(10532);

		x = y = 0;
		for (int64_t i = 0; i < iterations_count; ++i) {
			move_coords();
			draw_coords();
		}
	}

	void on_key_press(SDL_Event& e) {
		switch (e.key.keysym.sym) {
			case SDLK_UP: {
				h_min_map += move_factor;
				h_max_map += move_factor;
				break;
			}
			case SDLK_DOWN: {
				h_min_map -= move_factor;
				h_max_map -= move_factor;
				break;
			}
			case SDLK_RIGHT: {
				w_min_map += move_factor;
				w_max_map += move_factor;
				break;
			}
			case SDLK_LEFT: {
				w_min_map -= move_factor;
				w_max_map -= move_factor;
				break;
			}
			case SDLK_1: {
				iterations_count -= 10000;
				break;
			}
			case SDLK_2: {
				iterations_count += 10000;
				break;
			}
			case SDLK_3: {
				w_min_map += zoom_factor;
				w_max_map -= zoom_factor;
				h_min_map += zoom_factor;
				h_max_map -= zoom_factor;
				zoom_factor *= zoomed_down_factor;
				move_factor *= zoomed_down_factor;
				break;
			}
			case SDLK_4: {
				w_min_map -= zoom_factor;
				w_max_map += zoom_factor;
				h_min_map -= zoom_factor;
				h_max_map += zoom_factor;
				zoom_factor /= zoomed_down_factor;
				move_factor /= zoomed_down_factor;
				break;
			}
		}
	}
};