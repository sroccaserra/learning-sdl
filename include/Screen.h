#ifndef SCREEN_H
#define SCREEN_H

#include <cstddef>
#include <random>

class Screen {
    public:
        int width;
        int height;
        int depth = 24;
        int bytesPerPixel = 3;
        uint8_t* pixels;

        Screen(int width, int height);

        ~Screen();

        void initPixels();

        int rowSizeInBytes();

    private:
        std::uniform_int_distribution<int> uniform_dist;
        std::default_random_engine engine;

        int getRandomByte();
};

#endif // SCREEN_H
