#ifndef RESOURCES_H
#define RESOURCES_H

#include "Graph.hpp"
#include <SDL2/SDL.h>
#include <vector>

namespace TextureType
{
    const int STATIC = 0;
    const int DYNAMIC = 1;
};

enum BlockType
{
    BACKGROUND,
    BASE, // (NONE)
    SOURCE,    
    DESTINATION,   
    BLOCK,
    PATH,
    ANIMATE,
};

extern int gridSize;
extern int windowHeight;
extern int windowWidth;

enum Colors
{
    BLACK,      // BackGround
    WHITE,      // Base
    GREEN,      // Source    
    BLUE ,      // Target
    DARK_GRAY,  // Block
    DARK_GREEN, // Path
    
    LAVENDAR_INDIGO = 5,
    AQUAMARINE_BLUE = 22,
};

enum Direction
{
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST
};

enum Connect
{
    C4,
    C8,
};

extern std::vector<SDL_Color> colors;
extern std::vector<std::vector<int>> xDir;
extern std::vector<std::vector<int>> yDir;


#endif