#include "SDL_ResourceManager.hpp"

int windowHeight;
int windowWidth;
int gridSize = 15;
int clip = 1;
int texttype = TextureType::STATIC;

int blockType = BlockType::SOURCE;

std::vector<SDL_Color> colors = {
    {0, 0, 0, 255},       // Black
    {255, 255, 255, 255}, // White
    {126, 200, 80, 255},  // Green
    {126, 200, 80, 255},  // Green
    {23, 23, 23, 255},    // Dark Gray

    {13, 124, 26, 255}, // DarkGreen

    {210, 108, 244, 255}, // Lavender Indigo
    {193, 123, 255, 255}, // Medium Slate Blue
    {174, 137, 255, 255}, // Light Pastel Purple
    {152, 150, 255, 255}, // Very Light Blue
    {127, 162, 255, 255}, // Carolina Blue
    {98, 173, 255, 255},  // French Sky Blue
    {60, 183, 255, 255},  // Vivid Sky Blue
    {0, 192, 255, 255},   // Capri
    {0, 201, 255, 255},   // Deep Sky Blue
    {0, 209, 255, 255},   // Vivid Cerulean
    {0, 216, 255, 255},   // Bright Cerulean
    {0, 223, 255, 255},   // Cyan Cornflower Blue
    {0, 229, 255, 255},   // Fresh Air
    {0, 235, 255, 255},   // Spiro Disco Ball
    {0, 241, 255, 255},   // Waterspout
    {41, 246, 255, 255},  // Electric Blue
    {81, 251, 255, 255},  // Blueribbon
    {109, 255, 250, 255}  // Aquamarine Blue
};

std::vector<int> textureFramesSizes;

void SDLApp::Event::handleEvents()
{
    switch (event.type)
    {
    case SDL_QUIT:
        isAppRunning = false;
        break;

    case SDL_MOUSEBUTTONDOWN:
        handleMouseEvents();
        break;

    case SDL_KEYDOWN:
        handleKeyboardEvents();
        break;

    case SDL_MOUSEMOTION:
        handleMouseMotionEvents();
        break;
    }
}

void SDLApp::Event::handleMouseMotionEvents()
{
    if (event.motion.state && event.button.button == SDL_BUTTON_LEFT)
    {
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);

        x = x / (windowWidth / gridSize);
        y = y / (windowHeight / gridSize);

        getGrid().getBlock(x, y).type = BlockType::BLOCK;

        SDL_RenderCopy(app.resources.getRenderer(),
                       app.resources.getBlockTexture(x * gridSize + y),
                       NULL,
                       &app.resources.getGrid().getBlock(x, y).cell);

        SDL_RenderPresent(app.resources.getRenderer());
    }
}

void SDLApp::Event::handleMouseEvents()
{
}

SDLApp::Event::Event(SDLApp &app) : app(app)
{
}

Grid &SDLApp::Event::getGrid()
{
    return app.resources.grid;
}

void SDLApp::Event::handleKeyboardEvents()
{
    if (event.key.keysym.sym == SDLK_d)
    {

        std::cout << "Algorithm : Depth first Search" << std::endl;
        getGrid().dfs(getGrid().getSourceVertex(),
                      getGrid().getTargetVertex(),
                      [this]()
                      { this->app.blockTextureType[texttype](); });
    }

    if (event.key.keysym.sym == SDLK_c)
        app.resources.clearGrid();

    if (event.key.keysym.sym == SDLK_b)
    {
        std::cout << "Algorithm : Breadth first Search" << std::endl;
        getGrid().bfs(getGrid().getSourceVertex(),
                      getGrid().getTargetVertex(),
                      [this]()
                      { this->app.blockTextureType[texttype](); });
    }

    if (event.key.keysym.sym == SDLK_p)
    {
        std::cout << "Algorithm : Path Reconstruction " << std::endl;
        blockType = BlockType::PATH;
        getGrid().dijkstra(getGrid().getSourceVertex(),
                           getGrid().getTargetVertex());

        getGrid().reconstructPath(getGrid().getTargetVertex(),
                                  [this]()
                                  { this->app.blockTextureType[texttype](); });
    }

    if (event.key.keysym.sym == SDLK_a)
    {
        app.resources.getGrid().getBlock(app.resources.getGrid().sourceVertex).type = static_cast<BlockType>(BlockType::ANIMATE * 2 + 1);
        app.resources.getGrid().getBlock(app.resources.getGrid().targetVertex).type = static_cast<BlockType>(BlockType::ANIMATE * 2 + 1);
        blockType = BlockType::ANIMATE;
        texttype = TextureType::DYNAMIC;

        SDL_RenderCopy(app.resources.getRenderer(),
                       app.resources.getBlockTexture(app.resources.getGrid().sourceVertex),
                       NULL,
                       &app.resources.getGrid().getBlock(app.resources.getGrid().sourceVertex).cell);

        SDL_RenderCopy(app.resources.getRenderer(),
                       app.resources.getBlockTexture(app.resources.getGrid().targetVertex),
                       NULL,
                       &app.resources.getGrid().getBlock(app.resources.getGrid().targetVertex).cell);

        SDL_RenderPresent(app.resources.getRenderer());
    }

    if (event.key.keysym.sym == SDLK_q)
    {
        app.resources.getGrid().getBlock(app.resources.getGrid().sourceVertex).type = BlockType::SOURCE;
        app.resources.getGrid().getBlock(app.resources.getGrid().targetVertex).type = BlockType::DESTINATION;
        blockType = BlockType::SOURCE;
        texttype = TextureType::STATIC;

        SDL_RenderCopy(app.resources.getRenderer(),
                       app.resources.getBlockTexture(app.resources.getGrid().sourceVertex),
                       NULL,
                       &app.resources.getGrid().getBlock(app.resources.getGrid().sourceVertex).cell);

        SDL_RenderCopy(app.resources.getRenderer(),
                       app.resources.getBlockTexture(app.resources.getGrid().targetVertex),
                       NULL,
                       &app.resources.getGrid().getBlock(app.resources.getGrid().targetVertex).cell);

        SDL_RenderPresent(app.resources.getRenderer());
    }

    if (event.key.keysym.sym == SDLK_4)
    {
        std::cout << "set connection to 4" << std::endl;
        getGrid().setConnections(Connect::C4);
        getGrid().init();
    }

    if (event.key.keysym.sym == SDLK_8)
    {
        std::cout << "set connection to 8" << std::endl;
        getGrid().setConnections(Connect::C8);
        getGrid().init();
    }

    if (event.key.keysym.sym == SDLK_s)
    {
        app.resources.clearGrid();
        std::cout << "Choose source" << std::endl;
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);

        x = x / (windowWidth / gridSize);
        y = y / (windowHeight / gridSize);

        getGrid().setSourceVertex(x * gridSize + y);
        GridBlock &block = getGrid().getBlock(x, y);

        if (block.type != BlockType::BLOCK)
            block.type = (blockType == BlockType::ANIMATE) ? static_cast<BlockType>(BlockType::ANIMATE * 2 + 1) : BlockType::SOURCE;

        SDL_RenderCopy(app.resources.getRenderer(),
                       app.resources.getBlockTexture(x * gridSize + y),
                       NULL,
                       &app.resources.getGrid().getBlock(x, y).cell);

        SDL_RenderPresent(app.resources.getRenderer());
    }

    if (event.key.keysym.sym == SDLK_t)
    {
        std::cout << "Choose target" << std::endl;
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);

        x = x / (windowWidth / gridSize);
        y = y / (windowHeight / gridSize);

        getGrid().setTargetVertex(x * gridSize + y);

        GridBlock &block = getGrid().getBlock(x, y);
        if (block.type != BlockType::BLOCK)
            block.type = (blockType == BlockType::ANIMATE) ? static_cast<BlockType>(BlockType::ANIMATE * 2 + 1) : BlockType::DESTINATION;

        SDL_RenderCopy(app.resources.getRenderer(),
                       app.resources.getBlockTexture(x * gridSize + y),
                       NULL,
                       &app.resources.getGrid().getBlock(x, y).cell);

        SDL_RenderPresent(app.resources.getRenderer());
    }
}

void Resources::clearGrid()
{
    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            grid.getBlock(row, col).type = BlockType::BASE;
            grid.getBlock(row, col).frame = 0;

            SDL_RenderCopy(getRenderer(),
                           getBlockTexture(row * gridSize + col),
                           NULL,
                           &getGrid().getBlock(row, col).cell);
        }
    }

    SDL_RenderPresent(getRenderer());
}

Grid &Resources::getGrid()
{
    return grid;
}

SDL_Window *&Resources::getWindow()
{
    return window;
}

SDL_Renderer *&Resources::getRenderer()
{
    return renderer;
}

void SDLApp::renderGridBlockStatic()
{
    size_t currBlockId = resources.getGrid().currVertex;

    if (resources.getGrid().handleBlockType(currBlockId))
        return;

    resources.getGrid().changeBlockType(blockType, currBlockId);

    SDL_RenderCopy(resources.getRenderer(),
                   resources.getBlockTexture(currBlockId),
                   NULL,
                   &resources.getGrid().getBlock(currBlockId).cell);

    SDL_RenderPresent(resources.getRenderer());

    SDL_Delay(15);
}

void SDLApp::renderGridBlockDynamic()
{
    size_t currBlockId = resources.getGrid().currVertex;

    if (resources.getGrid().handleBlockType(currBlockId))
        return;

    resources.getGrid().changeBlockType(blockType, currBlockId);

    for (int row = 0; row < gridSize; row++)
    {
        for (int col = 0; col < gridSize; col++)
        {
            resources.getGrid().getBlock(row, col).frame = getManhattanDist(currBlockId / gridSize, currBlockId % gridSize, row, col);

            SDL_RenderCopy(resources.getRenderer(),
                           resources.getBlockTexture(row * gridSize + col),
                           NULL,
                           &resources.getGrid().getBlock(row, col).cell);
        }
    }

    SDL_RenderPresent(resources.getRenderer());

    SDL_Delay(20); 
}

int SDLApp::getManhattanDist(int x1, int y1, int x2, int y2)
{
    return abs(x2 - x1) + abs(y2 - y1);
}


SDLApp::SDLApp(std::string appName, int width, int height) : appName(appName)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "SDL failed to initialise : " << SDL_GetError;
        throw std::runtime_error(std::string(SDL_GetError()));
    }

    resources.getWindow() = SDL_CreateWindow(appName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,
                                             SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    SDL_GetWindowSize(resources.getWindow(), &windowWidth, &windowHeight);

    if (!resources.getWindow())
    {
        std::cerr << "SDL window failed to initialise : " << SDL_GetError();
        throw std::runtime_error(std::string(SDL_GetError()));
    }

    resources.getRenderer() = SDL_CreateRenderer(resources.window, PICK_UP_DEVICE_DRIVER, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!resources.getRenderer())
    {
        std::cerr << "SDL renderer failed to initialize : " << SDL_GetError();
        throw std::runtime_error(std::string(SDL_GetError()));
    }

    resources.load();

    blockTextureType.emplace_back([this]() { this->renderGridBlockStatic(); });
    blockTextureType.emplace_back([this]() { this->renderGridBlockDynamic(); });
    init = true;
}

void SDLApp::run() // event loop
{
    Event event(*this);
    while (isAppRunning)
    {
        while (SDL_WaitEvent(&event.event))
            event.handleEvents();
    }
}

SDLApp::~SDLApp()
{
    if (init)
    {
        SDL_DestroyWindow(resources.window);
        SDL_DestroyRenderer(resources.renderer);
        SDL_Quit();
    }
    resources.unload();
}

static SDL_Texture *createTextureFromImage(SDL_Renderer *renderer, char *imgPath)
{
    SDL_Surface *surface = SDL_LoadBMP(imgPath);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    return texture;
}
static SDL_Texture *createTextureFromColor(SDL_Renderer *renderer, SDL_Color color)
{
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET,
                                             windowWidth / gridSize, windowHeight / gridSize);
    if (!texture)
    {
        std::cerr << "SDL Texture unable to create texture : " << SDL_GetError() << std::endl;
        return nullptr;
    }

    SDL_SetRenderTarget(renderer, texture);
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}

void Resources::load()
{
    for (int type = BlockType::BACKGROUND; type <= BlockType::BLOCK; type++) // BlockType (Static Textures)
        blockTextures.emplace_back(createTextureFromColor(renderer, colors[type]));

    textureFramesSizes.push_back(blockTextures.size());

    for (int frame = Colors::LAVENDAR_INDIGO; frame <= Colors::AQUAMARINE_BLUE; frame++)
        blockTextures.emplace_back(createTextureFromColor(renderer, colors[frame]));

    textureFramesSizes.push_back(AQUAMARINE_BLUE - LAVENDAR_INDIGO);

    // SDL_SetRenderDrawColor(getRenderer(), colors[Colors::BLACK].r, colors[Colors::BLACK].g, colors[Colors::BLACK].b, 255);
    // SDL_RenderClear(getRenderer());

    grid.init();
    clearGrid();
}

SDL_Texture *Resources::getBlockTexture(int blockId)
{
    GridBlock block = getGrid().getBlock(blockId);

    if (block.type == BlockType::ANIMATE && block.type != BlockType::BLOCK)
    {
        if (block.frame >= textureFramesSizes[1])
            block.frame = textureFramesSizes[1] - 1;

        return blockTextures[block.type + block.frame];
    }

    return blockTextures[block.type];
}

void Resources::unload()
{
    for (int type = 0; type < blockTextures.size(); type++)
        SDL_DestroyTexture(blockTextures[type]);
}
