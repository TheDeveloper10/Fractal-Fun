#pragma once

#include "FF_Window.h"
#include "../Fractals/FractalCore.h"

#include <iostream>
#include <iomanip>
#include <chrono>

class FF_App {
private:
	FF_Window* window = nullptr;
public:
	~FF_App() {
		delete window;

		SDL_Quit();
	}

	void init(const char* title, int w, int h) {
		if (SDL_Init(SDL_INIT_VIDEO) != 0) {
			SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
			return;
		}

		std::cout << std::setprecision(3);
		
		window = new FF_Window(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
	}

	void start(FractalCore& fractal) {
		std::ios_base::sync_with_stdio(false);

		fractal.init(window->getScreenImage());

		std::chrono::high_resolution_clock::time_point start_time, end_time;

		SDL_Event e;
		// Check if user wants to quit
		while (true)
		{
			auto start_time = std::chrono::high_resolution_clock::now();

			// update texture
			window->clear();
			fractal.draw();
			window->updateTexture();

			auto end_time = std::chrono::high_resolution_clock::now();
			double secs = std::chrono::duration_cast<std::chrono::duration<double>>(end_time - start_time).count();
			std::cout << "Rendered frame for " << secs << "s.\n";

			// close logic
			SDL_PollEvent(&e);
			if (e.type == SDL_QUIT)
				break;
			else if (e.type == SDL_KEYDOWN) {
				//e.key.keysym
				fractal.on_key_press(e);
			}
		}
	}
};