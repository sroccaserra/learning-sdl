#include <cstddef>
#include <random>

class Screen {
    public:
        int width;
        int height;
        int depth = 24;
        int bytesPerPixel = 3;
        uint8_t* pixels;

        Screen(int width, int height) {
            this->width = width;
            this->height = height;
            int nbPixels = width*height;
            int dataSize = nbPixels*bytesPerPixel;
            this->pixels = new uint8_t[dataSize];
            initPixels();
        }

        ~Screen() {
            delete [] this->pixels;
            this->pixels = nullptr;
        }

        void initPixels() {
            std::random_device device;
            std::default_random_engine engine(device());
            std::uniform_int_distribution<int> uniform_dist(0, 255);

            for (int x = 0; x<width; ++x) {
                for (int y = 0; y<height; ++y) {
                    int r = x%256;
                    int g = y%256;
                    int b = uniform_dist(engine);

                    int offset = (x+y*width)*bytesPerPixel;
                    pixels[offset] = r;
                    pixels[offset+1] = g;
                    pixels[offset+2] = b;
                }
            }
        }

        int rowSizeInBytes() {
             return width*bytesPerPixel;
        }
};
