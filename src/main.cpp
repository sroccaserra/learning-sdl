#include <iostream>
#include <SDL2/SDL.h>

#include "cleanup.h"
#include "res_path.h"

void logSDLError(std::ostream &os, const std::string &msg){
    os << msg << " error: " << SDL_GetError() << std::endl;
}

int main(int, char**){
    if (SDL_Init(SDL_INIT_VIDEO) != 0){
        std::cout << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window * const window = SDL_CreateWindow("Hello World!", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
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

    //SDL 2.0 now uses textures to draw things but SDL_LoadBMP returns a surface
    //this lets us choose when to upload or remove textures from the GPU
    std::string imagePath = getResourcePath() + "hello.bmp";
    SDL_Surface * const bmp = SDL_LoadBMP(imagePath.c_str());
    if (bmp == nullptr){
        cleanup(renderer, window);
        logSDLError(std::cout, "SDL_LoadBMP");
        SDL_Quit();
        return 1;
    }

    //To use a hardware accelerated texture for rendering we can create one from
    //the surface we loaded
    SDL_Texture * const texture = SDL_CreateTextureFromSurface(renderer, bmp);
    SDL_FreeSurface(bmp);
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
