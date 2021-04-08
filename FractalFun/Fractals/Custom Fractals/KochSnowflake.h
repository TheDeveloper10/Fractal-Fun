#pragma once

#include "../../Core/Math/AdditionalMath.h"
#include "../../Core/Math/Vector2D.h"

#include "../../Core/Drawing/Point2D.h"
#include "../../Core/Drawing/Graphics/Graphics.h"

#include "../FractalCore.h"

#include <thread>
#include <vector>

class KochSnowflake : public FractalCore {
public:
	int64_t iterations_count = 0;
	float line_rotation_angle = 60.0f;
	const size_t THREADS_COUNT = 8;
private:
	class Line {
	public:
		Vector2D start, end;

		Line(const Vector2D& start, const Vector2D& end) 
			: start(start), end(end) {}

		Line* segment(float line_rotation_angle) const {
			Line* lines = (Line*)malloc(4 * sizeof(Line));

			Vector2D p1 = Vector2D(
				start.x + (end.x - start.x) / 3.0f,
				start.y + (end.y - start.y) / 3.0f
			);
			Vector2D p2 = Vector2D(
				(end.x - start.x) / 3.0f,
				(end.y - start.y) / 3.0f
			);
			// @TODO: show with 30deg
			//p2.rotate(-60/90); // good idea for logo
			p2.rotate(line_rotation_angle);
			p2 += p1;
			Vector2D p3 = Vector2D(
				end.x + (start.x - end.x) / 3.0f,
				end.y + (start.y - end.y) / 3.0f
			);

			lines[0] = Line(start, p1);
			lines[1] = Line(p1, p2);
			lines[2] = Line(p2, p3);
			lines[3] = Line(p3, end);

			return lines;
		}
	};
	
	std::vector<Line> lines;
	Graphics* g;

	void draw_part(size_t start, size_t end) {
		for (size_t i = start; i < end; ++i) {
			g->drawLine(
				Color(255, 255, 255),
				lines[i].start.toPoint2D(),
				lines[i].end.toPoint2D(),
				1
			);
		}
	}

	void new_lines() {
		const float w_inner_factor = 0.22f,
			h_inner_factor = 0.22f,
			x_displ = 0.0f,
			y_displ = 60.0f;
		Vector2D a = Vector2D(m_image->getWidth() * w_inner_factor + x_displ, m_image->getHeight() * h_inner_factor + y_displ),
			b = Vector2D(m_image->getWidth() * (1.0f - w_inner_factor) + x_displ, m_image->getHeight() * h_inner_factor + y_displ),
			c = Vector2D(m_image->getWidth() / 2.0f + x_displ, m_image->getHeight() * (1.0f - h_inner_factor) + y_displ);

		lines.push_back(Line(a, b));
		lines.push_back(Line(b, c));
		lines.push_back(Line(c, a));
	}
public:
	void draw() override {
		if (lines.size() <= 0)
		new_lines();

		Graphics temp = Graphics(m_image);
		g = &temp;

		if (lines.size() >= THREADS_COUNT * 2) {
			size_t p = lines.size() / THREADS_COUNT;

			std::vector<std::thread> threads;
			for (size_t i = 0; i < lines.size(); i += p) {
				size_t start = i;
				size_t end = std::min(i + p, lines.size());
				threads.push_back(
					std::thread(&KochSnowflake::draw_part, this, start, end)
				);
			}

			for (size_t i = 0; i < threads.size(); ++i)
				threads[i].join();
		}
		else
			draw_part(0, lines.size());
	}

private:
	void reset_lines() {
		// reset (the fill part happens in draw)
		lines.clear();
		iterations_count = 0;
	}

	void segment_lines() {
		std::vector<Line> newLines;

		for (Line line : lines) {
			Line* newLines_ = line.segment(line_rotation_angle);

			newLines.push_back(newLines_[0]);
			newLines.push_back(newLines_[1]);
			newLines.push_back(newLines_[2]);
			newLines.push_back(newLines_[3]);
		}

		++iterations_count;

		lines.clear();
		lines = newLines;
	}

	void rotate_lines(float rot) {
		line_rotation_angle += rot;

		size_t iterations_count_backup = iterations_count;
		reset_lines();
		new_lines();
		for (size_t i = 0;i < iterations_count_backup;++i)
			segment_lines();
	}
public:

	void on_key_press(SDL_Event& e) {
		switch (e.key.keysym.sym) {
			case SDLK_1: {
				segment_lines();
				break;
			}
			case SDLK_2: {
				reset_lines();
				break;
			}
			case SDLK_3: {
				rotate_lines(-2.0f);
				break;
			}
			case SDLK_4: {
				rotate_lines(2.0f);
				break;
			}
		}
	}
};