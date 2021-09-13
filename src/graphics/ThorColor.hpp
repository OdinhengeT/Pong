#ifndef COLOR_RGB_H
#define COLOR_RGB_H

#include <stdint.h>

namespace ThorColor {
    
    using RGB = uint32_t;

    RGB getRGB(uint8_t red, uint8_t green, uint8_t blue);

}

#endif