#pragma once
#include <cmath>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <utility>
#include <SDL2/SDL.h>
#include "grid.hpp"
#include "resources.hpp"


#define PICK_UP_DEVICE_DRIVER -1

static bool init = false;
static bool isAppRunning = true;

static SDL_Texture *createTextureFromImage(SDL_Renderer *renderer, char *imgPath);
static SDL_Texture *createTextureFromColor(SDL_Renderer *renderer, SDL_Color color);

struct Resources
{
    Grid grid;
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::vector<SDL_Texture *> blockTextures;

    void load();
    void unload();

    Grid& getGrid();
    void clearGrid();
    SDL_Window* &getWindow();
    SDL_Renderer* &getRenderer();
    SDL_Texture *getBlockTexture(int textureNo);
};

class SDLApp
{

public :
    SDLApp(std::string, int width, int height);
    static int getManhattanDist(int x1, int y1, int x2, int y2);
    
    void renderGridBlockStatic();
    void renderGridBlockDynamic();
    void run();
    void renderScene();

    ~SDLApp();

    struct Event
    {
        SDL_Event event;
        SDLApp &app;
        Event(SDLApp &app);
        Grid &getGrid();
        void handleEvents();
        void handleMouseEvents();
        void handleMouseMotionEvents();
        void handleKeyboardEvents();
    };

public:
    std::string appName;
    Resources resources;
    std::vector< std::function<void()> > blockTextureType;
};