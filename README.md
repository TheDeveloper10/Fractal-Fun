# Fractal-Fun
C++/SDL2 project to easily visualize fractals(like Mandelbrot Set, Julia Set, Burning Ship Fractal, Koch Snowflake, etc.) on CPU

## How to run it?
You need Visual Studio 2015 and above and you can just open the .sln file.

## How to see different fractals and colors?
* You can change to the fractal you want to see in Source.cpp - simply remove the
comment from the fractal you want to see and run it!
* If you want to change the coloring you can do that the same way you can change the
fractal - go in FractalColors.h (Fractals/FractalColor.h) and then remove the comment
from the coloring you want.

## What was the biggest issue I had to deal with?
It was the performance. It was an issue for fractals like the Mandelbrot Set, 
Julia Set, etc. I fixed the problem by making them on multiple threads and using AVX2.

## Screenshots
![alt text](https://thedevelopers.tech/images/pages/fractal_fun/screenshots/screenshot1.jpg)
![alt text](https://thedevelopers.tech/images/pages/fractal_fun/screenshots/screenshot2.jpg)
![alt text](https://thedevelopers.tech/images/pages/fractal_fun/screenshots/screenshot3.jpg)
![alt text](https://thedevelopers.tech/images/pages/fractal_fun/screenshots/screenshot4.jpg)
![alt text](https://thedevelopers.tech/images/pages/fractal_fun/screenshots/screenshot5.jpg)
For more screenshots: 
https://thedevelopers.tech/fractal_fun

## LICENSE
Apache-2.0 License
