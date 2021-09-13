#include "ThorColor.hpp"

using namespace ThorColor;

RGB getRGB(uint8_t red, uint8_t green, uint8_t blue) {
    uint32_t result = 0;
    result = (0<<24) | (red<<16) | (green<<8) | (blue);
    return result;
}