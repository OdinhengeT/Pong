#include "frame.hpp"
#include "fonts/bitFont.hpp"
#include <iostream>

uint32_t makeRGB(uint8_t red, uint8_t green, uint8_t blue) {
    // SDL uses RGBA format (0xRRGGBBAA), we'll use full alpha
    return (red << 24) | (green << 16) | (blue << 8) | 0xFF;
}

Frame::Frame(int width, int height) : width(width), height(height) {
    pixels = new uint32_t[width * height]();
}

Frame::~Frame() {
    delete[] pixels;
}

void* Frame::getPixels() {
    return this->pixels;
}

void Frame::clear() {
    for (int i = 0; i < height * width; i++) {
        pixels[i] = 0xFF000000; // Black with full alpha
    }
}

uint32_t Frame::get_pixelAt(unsigned int x, unsigned int y) {
    if (x >= width || y >= height) {
        std::cout << "Warning: Attempted to access pixel outside screen" << std::endl;
        return 0;
    }
    // Note: No Y-flip needed for SDL, we use standard top-to-bottom ordering
    int index = x + width * y;
    return pixels[index];
}

void Frame::set_pixelAt(unsigned int x, unsigned int y, uint32_t RGB_Color) {
    if (x >= width || y >= height) {
        std::cout << "Warning: Attempted write to pixel outside screen" << std::endl;
        return;
    }
    // Note: No Y-flip needed for SDL
    int index = x + width * y;
    pixels[index] = RGB_Color;
}

void Frame::drawRectangle(int x, int y, int w, int h, uint32_t RGB_Color) {
    for (int row = x; row < x + w; row++) {
        for (int col = y; col < y + h; col++) {
            set_pixelAt(row, col, RGB_Color);
        }
    }
}

void Frame::drawCharacter(int x0, int y0, char character, int pixel_width, int pixel_height, uint32_t RGB_Color) {
    BitFont8Char pf_char = getBitFont8Char(character);
    uint64_t bitmask = 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ((pf_char & (bitmask << (i + j * 8))) >> (i + j * 8)) {
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

void Frame::renderToTexture(SDL_Renderer* renderer, SDL_Texture* texture) {
    // Update the texture with our pixel data
    SDL_UpdateTexture(texture, nullptr, pixels, width * sizeof(uint32_t));
}
