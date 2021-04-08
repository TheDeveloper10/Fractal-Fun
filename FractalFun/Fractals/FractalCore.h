#pragma once

#include "../include/SDL.h"

#include "../Core/Drawing/Image.h"

class FractalCore {
protected:
	Image* m_image = nullptr;
public:

	void init(Image* image) {
		m_image = image;
	}

	virtual void draw() = 0;
	virtual void on_key_press(SDL_Event& e) = 0;
};