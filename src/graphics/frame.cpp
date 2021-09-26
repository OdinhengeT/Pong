#include <iostream>
#include <stdexcept>
#include <string>
#include <stdint.h>
#include <windows.h>

#include "frame.hpp"
#include "fonts/bitFont.hpp"

uint32_t makeRGB(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t result = 0;
    result = (0<<24) | (red<<16) | (green<<8) | (blue);
    return result;
}

Frame::Frame(int width, int height) {
    this->bitmap_info.bmiHeader.biSize = sizeof(this->bitmap_info.bmiHeader);
    this->bitmap_info.bmiHeader.biWidth = width;
    this->bitmap_info.bmiHeader.biHeight = height;
    this->bitmap_info.bmiHeader.biPlanes = 1;
    this->bitmap_info.bmiHeader.biBitCount = 32;
    this->bitmap_info.bmiHeader.biCompression = BI_RGB;
    pixels = new uint32_t[width * height + 1]();
}

Frame::~Frame() {
    delete[] pixels;
}

void* Frame::getPixels() {
    return this->pixels;
}

BITMAPINFO* Frame::getBitmap_info() {
    return &this->bitmap_info;
}

void Frame::clear() {
    for (int i = 0; i < this->bitmap_info.bmiHeader.biHeight * this->bitmap_info.bmiHeader.biWidth; i++) {
        pixels[i] = 0;
    }
}

// TODO Throw Exception if (x,y) is invalid
uint32_t Frame::get_pixelAt(unsigned int x, unsigned int y) {
    if (x > this->bitmap_info.bmiHeader.biWidth || y > this->bitmap_info.bmiHeader.biHeight) {
        std::cout << "Warning: Attempted to access pixel outside screen" << std::endl;
        return pixels[this->bitmap_info.bmiHeader.biWidth * this->bitmap_info.bmiHeader.biHeight];
    }

    int index = x + this->bitmap_info.bmiHeader.biWidth * (this->bitmap_info.bmiHeader.biHeight - y);
    return this->pixels[index];

}

// TODO Throw Exception if (x,y) is invalid
void Frame::set_pixelAt(unsigned int x, unsigned int y, uint32_t RGB_Color) {
    if (x > this->bitmap_info.bmiHeader.biWidth || y > this->bitmap_info.bmiHeader.biHeight) {
        std::cout << "Warning: Attempted write to pixel outside screen" << std::endl;
        return;
    }

    int index = x + this->bitmap_info.bmiHeader.biWidth * (this->bitmap_info.bmiHeader.biHeight - y);
    this->pixels[index] = RGB_Color;
}

void Frame::drawRectangle(int x, int y, int width, int height, uint32_t RGB_Color) {
    for (int row = x; row < x + width; row++) {
        for (int col = y; col < y + height; col++) {
            this->set_pixelAt(row, col,RGB_Color);
        }
    }
}

void Frame::drawCharacter(int x0, int y0, char character, int pixel_width, int pixel_height, uint32_t RGB_Color) {
    BitFont8Char pf_char = getBitFont8Char(character);
    uint64_t bitmask = 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ( (pf_char & (bitmask << (i + j * 8))) >> (i + j * 8) ) {
                drawRectangle(x0 + i * pixel_width, y0 + j * pixel_height, pixel_width, pixel_height, RGB_Color);
            }
        }
    }
}

void Frame::drawString(int x0, int y0, std::string string, int pixel_width, int pixel_height, uint32_t RGB_Color) {
    for (unsigned int i = 0; i < string.size(); i++) {
        drawCharacter(x0 + (i * 8 * pixel_width), y0, string[i], pixel_width, pixel_height, RGB_Color);
    }
}

