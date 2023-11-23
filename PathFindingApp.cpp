//HEADER FILES
#include <iostream>
#include <thread>
#include <string>
#include "SDL_ResourceManager.hpp"

//ENTRY POINT OF ALL FUNCTIONS
int SDL_main(int argc, char** argv)
{
    SDLApp app("PathFinding", 800, 800);

    app.run();

    return EXIT_SUCCESS;
}