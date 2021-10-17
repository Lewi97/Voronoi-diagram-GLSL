#include <raylib.h>

#include "VoronoiDiagram.h"

int main()
{
    // window dimension constants 
    const int screenWidth = 1024;
    const int screenHeight = 512;

    // create window
    InitWindow(screenWidth, screenHeight, "Voronoi Shader");
    
    // create simulation object
    VoronoiDiagram simulation(screenWidth, screenHeight);
    
    while (!WindowShouldClose())
    {
        // run simulation while window open
        simulation.run();
    }
    
    CloseWindow();

    return 0;
}