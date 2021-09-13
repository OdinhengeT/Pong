#ifndef FRAME_H
#define FRAME_H

#include <string>
#include <stdint.h>
#include <windows.h>

#include "../qShapes/qShapes.hpp"
#include "../fonts/thorFont.hpp"

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

        uint32_t& operator()(unsigned int x, unsigned int y);

        void clear();

        void drawLine(qShapes::qLine2D line, uint32_t RGB_Color);

        void drawTriangle(qShapes::qTriangle2D triangle, uint32_t RGB_Color);

        void drawRectangle(qShapes::qRectangle2D rectangle, uint32_t RGB_Color);

        void drawCharacter(int x0, int y0, char character, int pixel_width, int pixel_height, uint32_t RGB_Color);

        void drawString(int x0, int y0, std::string string, int pixel_width, int pixel_height, uint32_t RGB_Color);
};

#endif