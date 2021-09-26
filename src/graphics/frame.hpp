#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <stdint.h>
#include <windows.h>

#include "fonts/bitFont.hpp"

uint32_t makeRGB(uint8_t red, uint8_t green, uint8_t blue);

class Frame {
    private:
        BITMAPINFO bitmap_info;
        uint32_t* pixels;
    public:
        Frame(int width, int height);
        ~Frame();

        void* getPixels();
        BITMAPINFO* getBitmap_info();

        void clear();

        uint32_t get_pixelAt(unsigned int x, unsigned int y);

        void set_pixelAt(unsigned int x, unsigned int y, uint32_t RGB_Color);

        void drawRectangle(int x, int y, int width, int height, uint32_t RGB_Color);

        void drawCharacter(int x, int y, char character, int pixel_width, int pixel_height, uint32_t RGB_Color);

        void drawString(int x, int y, std::string string, int pixel_width, int pixel_height, uint32_t RGB_Color);
};

#endif