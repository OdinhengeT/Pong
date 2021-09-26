#include <iostream>
#include <memory>

#include "../src/entity/entity.hpp"
#include "../src/graphics/frame.hpp"
#include "../src/window/window.hpp"

int main() {
    std::cout << "Creating Window" << std::endl;

    auto pWindow = std::unique_ptr<Window>(new Window(L"Pong", 1440, 900));

    Frame* frame = new Frame(1440, 900);
 
    bool running = true; 
    while (running) {

        if (!pWindow->MsgProc()) {
            std::cout << "Closing Window" << std::endl;
            running = false;
        }

        pWindow->drawFrame(frame->getBitmap_info(), frame->getPixels());

    }

    delete frame;

    return 0;
}