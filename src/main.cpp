#include <iostream>
#include <random>

#include <SDL2/SDL.h>

#include "cleanup.h"
#include "res_path.h"

int width = 256;
int height = 256;
int depth = 24;
int bytesPerPixel = depth/8;
int pitch = width*bytesPerPixel;
Uint32 pixelFormat = SDL_PIXELFORMAT_RGB24;

int nbPixels = width*height;
int dataSize = nbPixels*bytesPerPixel;
Uint8 * pixels = new Uint8[dataSize];


void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    int pixelSize = 2;
    SDL_Window * const window = SDL_CreateWindow("Hello World!", 100, 100, width*pixelSize, height*pixelSize, SDL_WINDOW_SHOWN);
    if (window == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    //Create a renderer that will draw to the window, -1 specifies that we want to load whichever
    //video driver supports the flags we're passing
    //Flags: SDL_RENDERER_ACCELERATED: We want to use hardware accelerated rendering
    //SDL_RENDERER_PRESENTVSYNC: We want the renderer's present function (update screen) to be
    //synchronized with the monitor's refresh rate
    SDL_Renderer * const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    std::random_device r;
    std::default_random_engine e1(r());
    std::uniform_int_distribution<int> uniform_dist(0, 255);
    for (int x = 0; x<width; ++x) {
        for (int y = 0; y<height; ++y) {
            int r = x % 256;
            int g = y % 256;
            int b = uniform_dist(e1);

            int offset = (x+y*width)*bytesPerPixel;
            pixels[offset] = r;
            pixels[offset+1] = g;
            pixels[offset+2] = b;
        }
    }

    SDL_Surface * const surface = SDL_CreateRGBSurfaceWithFormatFrom(pixels, width, height, depth, pitch, pixelFormat);
    if (surface == nullptr){
        cleanup(renderer, window);
        logSDLError(std::cout, "Surface creation failed.");
        SDL_Quit();
        return 1;
    }

    //To use a hardware accelerated texture for rendering we can create one from
    //the surface we loaded
    SDL_Texture * const texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == nullptr){
        cleanup(renderer, window);
        logSDLError(std::cout, "SDL_CreateTextureFromSurface");
        SDL_Quit();
        return 1;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    SDL_Event event;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                quit = true;
            }
            if (event.type == SDL_KEYDOWN){
                quit = true;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN){
                quit = true;
            }
        }
    }

    cleanup(texture, renderer, window);
    SDL_Quit();

    return 0;
}
