#pragma once

#include <cstdint>
#include <cstring>
#include "../Image.h"

class Image;

class Graphics {
private:
	Image* m_image;
public:

	Graphics(Image* image) 
		: m_image(image) { }

private:
	// set pixel that avoids the check
	void set_pixel(Point2D loc, Color col) {
		/*size_t mem_loc = loc.y * m_image->getWidth() * m_image->getChannels() + loc.x * m_image->getChannels();
		m_image->m_data[mem_loc + 1] = col.b;
		m_image->m_data[mem_loc + 2] = col.g;
		m_image->m_data[mem_loc + 3] = col.r;*/
		m_image->setPixel(loc, col);
	}
	// set pixel that avoids the check
	void set_pixel(size_t mem_loc, Color col) {
		m_image->m_data[mem_loc + 1] = col.b;
		m_image->m_data[mem_loc + 2] = col.g;
		m_image->m_data[mem_loc + 3] = col.r;
	}
public:

	void fill(uint8_t value) {
		memset(m_image->m_data, value, m_image->getWidth() * m_image->getHeight() * m_image->getChannels() * sizeof(uint8_t));
	}

	void fill(Color color) {
		size_t steps = m_image->getHeight() * m_image->getWidth() * m_image->getChannels();
		for (size_t i = 0; i < steps; i += m_image->getChannels()) {
			m_image->m_data[i] = 255; // alpha
			m_image->m_data[i+1] = color.b; // blue
			m_image->m_data[i+2] = color.g; // green
			m_image->m_data[i+3] = color.r; // red
		}
	}

	void drawTriangle(const Color& col, const Point2D& a, const Point2D& b, const Point2D& c) {
		this->drawLine(col, a, b, 1);
		this->drawLine(col, b, c, 1);
		this->drawLine(col, c, a, 1);
	}

	// https://en.wikipedia.org/wiki/Digital_differential_analyzer_(graphics_algorithm)
	void drawLine(const Color& col, const Point2D& start, const Point2D& end, size_t thickness) {
		float x1 = start.x, y1 = start.y;
		float x2 = end.x, y2 = end.y, dx, dy, step;

		dx = (x2 - x1);
		dy = (y2 - y1);
		
		size_t x_step = 0, y_step = 0; // for thickness
		if (fabs(dx) >= fabs(dy)) {
			step = fabs(dx);
			y_step = 1;
		}
		else {
			step = fabs(dy);
			x_step = 1;
		}
		dx = dx / step;
		dy = dy / step;
		float x = x1;
		float y = y1;
		int i = 1;
		while (i <= step) {
			Point2D point(x - x_step * (thickness / 2), y - y_step * (thickness / 2));
			for (size_t thickness_ = thickness; thickness_ > 0; --thickness_) {
				set_pixel(point, col);
				point.x += x_step;
				point.y += y_step;
			}
			
			x = x + dx;
			y = y + dy;
			i = i + 1;
		}
	}
};