#pragma once

#define FF_IGNORE_EXCEPTIONS 1

#include <cstring>

#include "Color.h"
#include "Point2D.h"

// 4 channel(R,G,B,A) image
class Image {
private:
	const uint8_t m_channels = 4;
	uint8_t* m_data;
	size_t m_width, m_height;
	
	friend class FF_Window;
	friend class Graphics;
public:

	Image(size_t width, size_t height) : 
		m_width(width), m_height(height) {
		// allocate memory of image (equivalent to malloc(...))
		m_data = (uint8_t*)operator new(m_width * m_height * m_channels);
	}

	~Image() {
		// free image data (equivalent to free(...))
		operator delete(m_data);
	}

	_NODISCARD inline const size_t getWidth() const {
		return m_width;
	}
	_NODISCARD inline const size_t getHeight() const {
		return m_height;
	}
	_NODISCARD inline const size_t getChannels() const {
		return m_channels;
	}
	_NODISCARD inline const uint8_t* const getDataPtr() const {
		return m_data;
	}
	_NODISCARD inline uint8_t* const getDataPtr() {
		return m_data;
	}

	inline void setPixel(const Point2D& point, const Color& color) {
		if (point.x >= m_width || point.y >= m_height) {
#if FF_IGNORE_EXCEPTIONS == 0
			throw "Coordinates out of range!";
#else
			return;
#endif
		}

		size_t coords_1d = point.y * m_width * m_channels + 
							point.x * m_channels;

		m_data[coords_1d] = 255;
		m_data[coords_1d + 1] = color.b;
		m_data[coords_1d + 2] = color.g;
		m_data[coords_1d + 3] = color.r;
	}

	_NODISCARD inline const Color getPixel(const Point2D& point) const {
		if (point.x >= m_width || point.y >= m_height) {
#if FF_IGNORE_EXCEPTIONS == 0
			throw "Coordinates out of range!";
#else
			return Color(0,0,0);
#endif
		}

		size_t coords_1d = point.y * m_width * m_channels +
			point.x * m_channels;

		return Color(m_data[coords_1d + 3], m_data[coords_1d + 2], m_data[coords_1d + 1]);
	}

	void clear() {
		memset(m_data, 0, m_width * m_height * m_channels);
	}
};