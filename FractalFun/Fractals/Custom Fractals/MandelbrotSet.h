#pragma once

#include "../../Core/Math/Complex.h"
#include "../../Core/Math/AdditionalMath.h"

#include "../FractalCore.h"
#include "../FractalColors.h"
#include "../../Core/Drawing/Graphics/Graphics.h"

#include <vector>
#include <thread>

class MandelbrotSet : public FractalCore {
public:
	size_t max_iterations = 64;
	double fs_w_min = -1.5, fs_w_max = 0.5;
	double fs_h_min = -1, fs_h_max = 1;
	const size_t THREADS_COUNT = 8;

	double move_sensitivity = 0.02f;
	double move_sensitivity_down_factor = 0.98f;
	double zoom_sensitivity = 0.02f;
	double zoom_sensitivity_down_factor = 0.98f;
private:
	void draw_part(size_t colStart, size_t colEnd) {
		for (size_t i = colStart; i < colEnd; ++i) {
			for (size_t j = 0; j < m_image->getHeight(); ++j) {
				float x_map = map(i, 0, m_image->getWidth(), fs_w_min, fs_w_max);
				float y_map = map(j, 0, m_image->getHeight(), fs_h_min, fs_h_max);

				Color col;

				Complex z(0, 0);
				Complex c(x_map, y_map);
				
				int n = 0;

				while (abs(z.lengthSqr()) < 4.0f && n < max_iterations) {
					z = (z * z) + c;
					++n;
				}


				m_image->setPixel(Point2D(i, j), color_based_on_iteration(n, max_iterations));
			}
		}
	}
public:
	void draw() {
		std::vector<std::thread> threads;

		size_t k = m_image->getWidth() / THREADS_COUNT;
		for (size_t i = 0; i < THREADS_COUNT; ++i) {
			//threads.draw_part(k);
			threads.push_back(
				std::thread(&MandelbrotSet::draw_part, this, i * k, (i + 1) * k)
			);
		}

		for (size_t i = 0; i < THREADS_COUNT; ++i)
			threads[i].join();
	
		
	
	}

	void on_key_press(SDL_Event& e) {
		switch (e.key.keysym.sym) {
		case SDLK_UP: {
			fs_h_min -= move_sensitivity;
			fs_h_max -= move_sensitivity;
			break;
		}
		case SDLK_DOWN: {
			fs_h_min += move_sensitivity;
			fs_h_max += move_sensitivity;
			break;
		}
		case SDLK_LEFT: {
			fs_w_min -= move_sensitivity;
			fs_w_max -= move_sensitivity;
			break;
		}
		case SDLK_RIGHT: {
			fs_w_min += move_sensitivity;
			fs_w_max += move_sensitivity;
			break;
		}
		case SDLK_1: { // zoom in
			fs_w_min += zoom_sensitivity;
			fs_w_max -= zoom_sensitivity;
			fs_h_min += zoom_sensitivity;
			fs_h_max -= zoom_sensitivity;
			zoom_sensitivity *= zoom_sensitivity_down_factor;
			move_sensitivity *= move_sensitivity_down_factor;
			break;
		}
		case SDLK_2: { // zoom out
			fs_w_min -= zoom_sensitivity;
			fs_w_max += zoom_sensitivity;
			fs_h_min -= zoom_sensitivity;
			fs_h_max += zoom_sensitivity;
			zoom_sensitivity /= zoom_sensitivity_down_factor;
			move_sensitivity /= move_sensitivity_down_factor;
			break;
		}
		case SDLK_3: {
			max_iterations += 16;
			break;
		}
		case SDLK_4: {
			max_iterations -= 16;
			break;
		}
		}
	}
};