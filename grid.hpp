#pragma once

#include <vector>
#include <string>
#include "Graph.hpp"
#include <SDL2/SDL.h>
#include "resources.hpp"


struct GridBlock
{
    int frame = 0;
    BlockType type;
    SDL_Rect cell;
   

    GridBlock(size_t w = windowWidth / gridSize,
              size_t h = windowHeight / gridSize,
              size_t x = 0, size_t y = 0, BlockType type = BlockType::BASE);
};

struct Grid : public Graph<GridBlock>
{
    size_t numNodes;
    size_t nodeHeight;
    size_t nodeWidth;
    Connect connect;

    static bool isValid(int x, int y);

    Grid();

    void init();

    GridBlock &getBlock(int x, int y);

    GridBlock &getBlock(VertexId id);

    void setConnections(Connect connect);

    void changeBlockType(int blocktype, VertexId id);

    bool handleBlockType(VertexId id);

};

