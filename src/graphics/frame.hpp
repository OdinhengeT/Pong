#ifndef FRAME_HPP
#define FRAME_HPP

#include <SDL3/SDL.h>
#include <string>
#include <stdint.h>

uint32_t makeRGB(uint8_t red, uint8_t green, uint8_t blue);

class Frame {
private:
    int width;
    int height;
    uint32_t* pixels;

public:
    Frame(int width, int height);
    ~Frame();

    void* getPixels();
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    void clear();
    uint32_t get_pixelAt(unsigned int x, unsigned int y);
    void set_pixelAt(unsigned int x, unsigned int y, uint32_t RGB_Color);
    void drawRectangle(int x, int y, int width, int height, uint32_t RGB_Color);
    void drawCharacter(int x0, int y0, char character, int pixel_width, int pixel_height, uint32_t RGB_Color);
    void drawString(int x0, int y0, std::string string, int pixel_width, int pixel_height, uint32_t RGB_Color);

    // SDL3-specific method to render the frame to a texture
    void renderToTexture(SDL_Renderer* renderer, SDL_Texture* texture);
};

#endif
