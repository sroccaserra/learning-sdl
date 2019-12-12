#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <cstddef>
#include <random>

class Framebuffer {
    public:
        int width;
        int height;
        int depth = 24;
        int bytesPerPixel = 3;
        uint8_t* pixels;

        Framebuffer(const int width, const int height);

        ~Framebuffer();

        const int rowSizeInBytes();

        void initPixels();
        void update();

    private:
        std::uniform_int_distribution<int> uniform_dist;
        std::default_random_engine engine;

        int getRandomByte();
};

#endif // FRAMEBUFFER_H
