#pragma once

#include "SDL.h"

#include "Drawing/Image.h"

class FF_Window {
private:
	SDL_Window* window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Texture* texture = nullptr;
	Image* image = nullptr;
public:

	FF_Window(const char *title, int x, int y, int w, int h, Uint32 flags) {
		window = SDL_CreateWindow(title, x, y, w, h, flags);
		if (window == nullptr) {
			SDL_Log("Unable to initialize SDL window: %s", SDL_GetError());
			return;
		}

		renderer = SDL_CreateRenderer(window, -1, 0);
		if (renderer == nullptr) {
			SDL_Log("Unable to initialize SDL renderer: %s", SDL_GetError());
			return;
		}

		texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR32, SDL_TEXTUREACCESS_STATIC, w, h);
		image = new Image(w, h);
	}

	~FF_Window() {
		SDL_DestroyTexture(texture);
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);

		delete image;
	}

	void clear() {
		image->clear();
		SDL_RenderClear(renderer);
	}

	void updateTexture() {
		SDL_UpdateTexture(texture, nullptr, image->m_data, image->m_width * image->m_channels);
		SDL_RenderCopy(renderer, texture, nullptr, nullptr);
		SDL_RenderPresent(renderer);
	}

	Image* const getScreenImage() {
		return image;
	}
};