#include <iostream>
#include <random>

#include <SDL2/SDL.h>

#include "cleanup.h"
#include "res_path.h"
#include "Framebuffer.h"

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

int draw(SDL_Renderer * renderer, SDL_Surface * surface) {
    SDL_Texture * const texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr){
        return 1;
    }

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    cleanup(texture);
    SDL_RenderPresent(renderer);

    return 0;
}

int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    Framebuffer framebuffer(256, 256);
    int pixelSize = 2;
    SDL_Window * const window = SDL_CreateWindow(
        "Hello World!",
        100,
        100,
        framebuffer.width*pixelSize,
        framebuffer.height*pixelSize,
        SDL_WINDOW_SHOWN
    );
    if (window == nullptr){
        std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Renderer * const renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr){
        logSDLError(std::cout, "CreateRenderer");
        cleanup(window);
        SDL_Quit();
        return 1;
    }

    int pitch = framebuffer.rowSizeInBytes();
    Uint32 pixelFormat = SDL_PIXELFORMAT_RGB24;
    SDL_Surface *const surface = SDL_CreateRGBSurfaceWithFormatFrom(
        framebuffer.pixels,
        framebuffer.width,
        framebuffer.height,
        framebuffer.depth,
        pitch,
        pixelFormat
    );
    if (surface == nullptr){
        cleanup(renderer, window);
        logSDLError(std::cout, "Surface creation failed.");
        SDL_Quit();
        return 1;
    }

    int drawResult = draw(renderer, surface);
    if (drawResult != 0) {
        cleanup(renderer, window);
        logSDLError(std::cout, "SDL_CreateTextureFromSurface");
        SDL_Quit();
        return 1;
    }

    SDL_Event event;
    bool quit = false;
    while (!quit){
        while (SDL_PollEvent(&event)){
            if (event.type == SDL_QUIT){
                quit = true;
            }
        }

        framebuffer.update();
        draw(renderer, surface);
    }

    cleanup(surface, renderer, window);
    SDL_Quit();

    return 0;
}
