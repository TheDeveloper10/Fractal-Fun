// https://stackoverflow.com/questions/53376811/linker-error-sdl-main-unresolved-in-main-getcmdline
#define SDL_MAIN_HANDLED

#include "Core/FF_App.h"

#include "Fractals/Custom Fractals/TreeFractal.h"
#include "Fractals/Custom Fractals/MandelbrotSet.h"
#include "Fractals/Custom Fractals/MandelbrotSet_AVX2.h"
#include "Fractals/Custom Fractals/JuliaSet.h"
#include "Fractals/Custom Fractals/JuliaSet_AVX2.h"
#include "Fractals/Custom Fractals/BurningShipFractal.h"
#include "Fractals/Custom Fractals/BurningShipFractal_AVX2.h"
#include "Fractals/Custom Fractals/BarnsleysFern.h"
#include "Fractals/Custom Fractals/KochSnowflake.h"
#include "Fractals/Custom Fractals/SierpinskisTriangle.h"

#include <iostream>

int main() {

    FF_App app;

    app.init("Fractal Fun", 960, 720);

    
    /*
        To get a different fractal:
            1. Comment the current active(TreeFractal in this case)
            2. Remove the comment from the fractal you want(e.g. Barnsley's Fern)
            3. Compile

        To get different coloring:
            1. Go to Fractals/FractalColors.h
            2. Comment the current coloring
            3. Remove the comment from the coloring you want
    */

    // Fractals using multithreading and AVX2(some of them - they have _AVX2)
    TreeFractal fractal = TreeFractal();
    //KochSnowflake fractal = KochSnowflake();
    //SierpinskisTriangle fractal = SierpinskisTriangle();
    //BarnsleysFern fractal = BarnsleysFern();
    //MandelbrotSet_AVX2 fractal = MandelbrotSet_AVX2();
    //JuliaSet_AVX2 fractal = JuliaSet_AVX2();
    //BurningShipFractal_AVX2 fractal = BurningShipFractal_AVX2();


    // Fractals using only multi-threading and not AVX2
    //MandelbrotSet fractal = MandelbrotSet();
    //JuliaSet fractal = JuliaSet();
    //BurningShipFractal fractal = BurningShipFractal();

    app.start(fractal);

    return 0;
}