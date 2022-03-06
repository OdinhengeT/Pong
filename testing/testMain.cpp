#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "../src/entity/entity.hpp"
#include "../src/graphics/frame.hpp"
#include "../src/window/window.hpp"

int main() {
    std::cout << "Creating Window" << std::endl;

    static const float TIME_STEP = 1.0f / 10.0f; // 1 / fps (delta time in seconds)

    auto pWindow = std::unique_ptr<Window>(new Window(L"Pong", 1440, 900));

    Frame* frame = new Frame(1440, 900);

    Entity one(200, 200, 80, 80, 10);
    Entity two(400, 200, 80, 80, 20);
    one.setVelocityX(10);
    two.setVelocityY(1);


    bool running = true; 
    while (running) {
        
        auto t0 = std::chrono::high_resolution_clock::now();
        auto t1 = t0 + std::chrono::microseconds(static_cast<int>(1000000.0f * TIME_STEP));

        if (!pWindow->MsgProc()) {
            std::cout << "Closing Window" << std::endl;
            running = false;
        }

        frame->clear();

        frame->drawString(50, 50, "ABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!=", 4, 6, makeRGB(255, 128, 0));

        frame->drawRectangle(
            static_cast<int>(one.getPositionX() + 0.5f), 
            static_cast<int>(one.getPositionY() + 0.5f), 
            static_cast<int>(one.getWidth() + 0.5f), 
            static_cast<int>(one.getHeight()) + 0.5f, 
            makeRGB(255, 255, 255)
        );
        frame->drawRectangle(
            static_cast<int>(two.getPositionX() + 0.5f), 
            static_cast<int>(two.getPositionY() + 0.5f), 
            static_cast<int>(two.getWidth() + 0.5f), 
            static_cast<int>(two.getHeight()) + 0.5f, 
            makeRGB(255, 255, 255)
        );

        one.timeStep(TIME_STEP);        
        two.timeStep(TIME_STEP);

        collisionHandler(one, two);

        pWindow->drawFrame(frame->getBitmap_info(), frame->getPixels());

        std::this_thread::sleep_until(t1);

    }

    delete frame;

    return 0;
}