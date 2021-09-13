#include <iostream>
#include <stdexcept>
#include <string>

#include "frame.hpp"

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
    pixels = new uint32_t[width * height + 1](); // The extra pixel is used as error return
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

uint32_t& Frame::operator()(unsigned int x, unsigned int y) {
    if (x > this->bitmap_info.bmiHeader.biWidth || y > this->bitmap_info.bmiHeader.biHeight) {
        std::cout << "Warning: Attempted to access pixel outside screen" << std::endl;
        return pixels[this->bitmap_info.bmiHeader.biWidth * this->bitmap_info.bmiHeader.biHeight];
    }

    int index = x + this->bitmap_info.bmiHeader.biWidth * (this->bitmap_info.bmiHeader.biHeight - y);
    return this->pixels[index];
}

void Frame::clear() {
    for (int i = 0; i < this->bitmap_info.bmiHeader.biHeight * this->bitmap_info.bmiHeader.biWidth; i++) {
        pixels[i] = 0;
    }
}

void Frame::drawLine(qShapes::qLine2D line, uint32_t RGB_Color) {
    for (int i = -1; i <=1; i++) {
        for (int j = -1; j <=1; j++) {
            (*this)(line.vert_1.getIntX() + i, line.vert_1.getIntY() + j) = makeRGB(255, 255, 255);
            (*this)(line.vert_2.getIntX() + i, line.vert_2.getIntY() + j) = makeRGB(255, 255, 255);
        }
    }

    int x0 = line.vert_1.getIntX();
    int x1 = line.vert_2.getIntX();
    int y0 = line.vert_1.getIntY();
    int y1 = line.vert_2.getIntY();

    int dx =  abs(x1-x0);
    int dy = -abs(y1-y0);
    int err = dx+dy; 

    while (true) {
        (*this)(x0, y0) = RGB_Color;
        if (x0 == x1 && y0 == y1) break;
        
        int e2 = 2*err;
        if (e2 >= dy) {
            err += dy;
            x0 += x0<x1 ? 1 : -1;
        }
        if (e2 <= dx) {
            err += dx;
            y0 += y0<y1 ? 1 : -1;
        }
    }

    //  Skriv om med tjocklek - ytterlinjer förskjutna i ortogonalled.
    //  Om centrum av en pixel ligger mellan linjerna så fylls den i.

}

void Frame::drawTriangle(qShapes::qTriangle2D triangle, uint32_t RGB_Color) {
    for (int i = -1; i <=1; i++) {
        for (int j = -1; j <=1; j++) {
            (*this)(triangle.vert_1.getIntX() + i, triangle.vert_1.getIntY() + j) = makeRGB(255, 255, 255);
            (*this)(triangle.vert_2.getIntX() + i, triangle.vert_2.getIntY() + j) = makeRGB(255, 255, 255);
            (*this)(triangle.vert_3.getIntX() + i, triangle.vert_3.getIntY() + j) = makeRGB(255, 255, 255);
        }
    }

    drawLine(qShapes::qLine2D(triangle.vert_1, triangle.vert_2), RGB_Color);
    drawLine(qShapes::qLine2D(triangle.vert_2, triangle.vert_3), RGB_Color);
    drawLine(qShapes::qLine2D(triangle.vert_3, triangle.vert_1), RGB_Color);
}

void Frame::drawRectangle(qShapes::qRectangle2D rectangle, uint32_t RGB_Color) {

    int x0 = rectangle.vert_1.getIntX();
    int y0 = rectangle.vert_1.getIntY();
    int width = (int) rectangle.width;
    int height = (int) rectangle.height;

    for (int row = x0; row < x0 + width; row++) {
        for (int col = y0; col < y0 + height; col++) {
            (*this)(row, col) = RGB_Color;
        }
    }
}

void Frame::drawCharacter(int x0, int y0, char character, int pixel_width, int pixel_height, uint32_t RGB_Color) {
    ThorFont8Char pf_char = getThorFont8Char(character);
    uint64_t bitmask = 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            if ( (pf_char & (bitmask << (i + j * 8))) >> (i + j * 8) ) {
                drawRectangle(qShapes::qRectangle2D(x0 + i * pixel_width, y0 + j * pixel_height, pixel_width, pixel_height), RGB_Color);
            }
        }
    }
}

void Frame::drawString(int x0, int y0, std::string string, int pixel_width, int pixel_height, uint32_t RGB_Color) {
    for (unsigned int i = 0; i < string.size(); i++) {
        drawCharacter(x0 + (i * 8 * pixel_width), y0, string[i], pixel_width, pixel_height, RGB_Color);
    }
}

