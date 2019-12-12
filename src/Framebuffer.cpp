#include "Framebuffer.h"

Framebuffer::Framebuffer(int width, int height) : width(width), height(height){
    int nbPixels = width*height;
    int dataSize = nbPixels*bytesPerPixel;
    pixels = new uint8_t[dataSize];

    std::random_device device;
    engine = std::default_random_engine(device());
    uniform_dist = std::uniform_int_distribution(0, 255);

    initPixels();
}

Framebuffer::~Framebuffer() {
    delete [] pixels;
    pixels = nullptr;
}

int Framebuffer::rowSizeInBytes() {
    return width*bytesPerPixel;
}

void Framebuffer::initPixels() {
    for (int x = 0; x<width; ++x) {
        for (int y = 0; y<height; ++y) {
            int r = x%256;
            int g = y%256;
            int b = getRandomByte();

            int offset = (x+y*width)*bytesPerPixel;
            pixels[offset] = r;
            pixels[offset+1] = g;
            pixels[offset+2] = b;
        }
    }
}

void Framebuffer::update() {
    initPixels();
}

int Framebuffer::getRandomByte() {
    return uniform_dist(engine);
}
