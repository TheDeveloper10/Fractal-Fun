#pragma once

#include "../../Core/Math/Complex.h"
#include "../../Core/Math/AdditionalMath.h"

#include "../FractalCore.h"
#include "../FractalColors.h"
#include "../../Core/Drawing/Graphics/Graphics.h"

#include <vector>
#include <thread>
#include <immintrin.h>

class JuliaSet_AVX2 : public FractalCore {
public:
	size_t max_iterations = 64;
	double fs_w_min = -1, fs_w_max = 1;
	double fs_h_min = -1, fs_h_max = 1;
	const size_t THREADS_COUNT = 8;

	double move_sensitivity = 0.02f;
	double move_sensitivity_down_factor = 0.98f;
	double zoom_sensitivity = 0.02f;
	double zoom_sensitivity_down_factor = 0.98f;

	Complex c = Complex(-0.835, -0.2321);
private:
	void draw_part(size_t colStart, size_t colEnd) {
		__m256d
			_zr, _zr2,
			_zi, _zi2;
		const __m256d 
					_cr = _mm256_set1_pd(c.real),
					_ci = _mm256_set1_pd(c.imaginary),
					_two = _mm256_set1_pd(2.0),
					_four = _mm256_set1_pd(4.0);

		__m256i _n,
			_if_result;
		const __m256i _one = _mm256_set1_epi64x(1),
			_max_iterations = _mm256_set1_epi64x(max_iterations + 1);


		for (size_t j = 0; j < m_image->getHeight(); ++j) {
			size_t i = colStart;
			for (
				Color* ptr = (Color*)(m_image->getDataPtr() + (j * m_image->getWidth() + colStart) * m_image->getChannels());
				i < colEnd;
				i += 4,
				ptr += 4) {
				
				// _zi = mapped y coord
				_zi = _mm256_set1_pd(
									map(j, 0, m_image->getHeight(), fs_h_min, fs_h_max)
								);

				// _zr = mapped x coord
				_zr = _mm256_set_pd(
									map(i + 0, 0, m_image->getWidth(), fs_w_min, fs_w_max),
									map(i + 1, 0, m_image->getWidth(), fs_w_min, fs_w_max),
									map(i + 2, 0, m_image->getWidth(), fs_w_min, fs_w_max),
									map(i + 3, 0, m_image->getWidth(), fs_w_min, fs_w_max)
								);

				// _n = 0; (si -> signed integer)
				_n = _mm256_setzero_si256();

			_loop_start:

				// _zi ^ 2 = _zi * _zi
				_zi2 = _mm256_mul_pd(_zi, _zi);
				// _zr ^ 2 = _zr * _zr
				_zr2 = _mm256_mul_pd(_zr, _zr);

				// @TODO: SHOW THE RESULT OF MOVING _zi2 & _zr2 CALCULATIONS HERE 

				// if (zr * zr + zi * zi < 4.0f && n < max_iterations)
				_if_result =
					// mask
					_mm256_and_si256(
						// and 2 conditions
						_mm256_and_si256(
							// cast pd(256) to si(256)
							_mm256_castpd_si256(
								// zr * zr + zi * zi < 4.0f
								_mm256_cmp_pd(
									_mm256_add_pd(_zr2, _zi2),
									_four,
									_CMP_LT_OQ
								)
							),
							// max_iterations > n
							_mm256_cmpgt_epi64(
								_max_iterations,
								_n
							)
						),
						_one
					);

				if (_if_result.m256i_i64[0] +
					_if_result.m256i_i64[1] +
					_if_result.m256i_i64[2] +
					_if_result.m256i_i64[3] <= 0)
					goto _loop_end;

				// zr^2 = zr * zr - zi * zi
				// zi^2 = 2 * zr * zi
				// (z * z)
				_zr2 = _mm256_sub_pd(_zr2, _zi2);
				_zi2 = _mm256_mul_pd(_mm256_mul_pd(_zr, _zi), _two);

				// zr = zr^2 + cr
				// zi = zi^2 + ci
				// z = (z * z) + c
				_zr = _mm256_add_pd(_zr2, _cr);
				_zi = _mm256_add_pd(_zi2, _ci);

				// _if_result contains 1s on the places that we actually
				// need to increment so we can use it to directly increment there
				_n = _mm256_add_epi64(_n, _if_result);

				goto _loop_start;
			_loop_end:

#if defined(_WIN32)
				* (ptr + 0) = color_based_on_iteration(_n.m256i_i64[3], max_iterations);
				*(ptr + 1) = color_based_on_iteration(_n.m256i_i64[2], max_iterations);
				*(ptr + 2) = color_based_on_iteration(_n.m256i_i64[1], max_iterations);
				*(ptr + 3) = color_based_on_iteration(_n.m256i_i64[0], max_iterations);
#elif defined(__linux__)
				* (ptr + 0) = color_based_on_iteration(_n[3], max_iterations);
				*(ptr + 1) = color_based_on_iteration(_n[2], max_iterations);
				*(ptr + 2) = color_based_on_iteration(_n[1], max_iterations);
				*(ptr + 3) = color_based_on_iteration(_n[0], max_iterations);
#endif
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
				std::thread(&JuliaSet_AVX2::draw_part, this, i * k, (i + 1) * k)
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
		case SDLK_a: {
			c.real -= 0.001f;
			break;
		}
		case SDLK_d: {
			c.real += 0.001f;
			break;
		}
		case SDLK_s: {
			c.imaginary -= 0.001f;
			break;
		}
		case SDLK_w: {
			c.imaginary += 0.001f;
			break;
		}
		}
	}
};