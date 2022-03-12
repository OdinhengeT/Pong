#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

#include "../src/entity/entity.hpp"
#include "../src/graphics/frame.hpp"
#include "../src/window/window.hpp"

int main() {
    std::cout << "Creating Window" << std::endl;

    static const float TIME_STEP = 1.0f / 60.0f; // 1 / fps (delta time in seconds)

    auto pWindow = std::unique_ptr<Window>(new Window(L"Pong", 1440, 900));

    Frame* frame = new Frame(1440, 900);

    Entity left(250, 250, 25, 400, 9000);
    Entity right(675, 250, 25, 400, 9000);
    Entity top(275, 250, 400, 25, 9000);
    Entity bot(275, 625, 400, 25, 9000);

    Entity one(350, 300, 50, 50, 10);
    Entity two(550, 310, 50, 50, 10);
    
    one.setVelocityX(10);
    //one.setVelocityY(50);
    two.setVelocityX(-10);
    //two.setVelocityY(-30);


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

        // Frame

        frame->drawRectangle(
            static_cast<int>(left.getPositionX() + 0.5f), 
            static_cast<int>(left.getPositionY() + 0.5f), 
            static_cast<int>(left.getWidth() + 0.5f), 
            static_cast<int>(left.getHeight()) + 0.5f, 
            makeRGB(255, 255, 255)
        );
        frame->drawRectangle(
            static_cast<int>(right.getPositionX() + 0.5f), 
            static_cast<int>(right.getPositionY() + 0.5f), 
            static_cast<int>(right.getWidth() + 0.5f), 
            static_cast<int>(right.getHeight()) + 0.5f, 
            makeRGB(255, 255, 255)
        );
        frame->drawRectangle(
            static_cast<int>(top.getPositionX() + 0.5f), 
            static_cast<int>(top.getPositionY() + 0.5f), 
            static_cast<int>(top.getWidth() + 0.5f), 
            static_cast<int>(top.getHeight()) + 0.5f, 
            makeRGB(255, 255, 255)
        );
        frame->drawRectangle(
            static_cast<int>(bot.getPositionX() + 0.5f), 
            static_cast<int>(bot.getPositionY() + 0.5f), 
            static_cast<int>(bot.getWidth() + 0.5f), 
            static_cast<int>(bot.getHeight()) + 0.5f, 
            makeRGB(255, 255, 255)
        );

        // Balls
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
/*
        collisionHandler(one, left);
        collisionHandler(one, right);
        collisionHandler(one, top);
        collisionHandler(one, bot);

        collisionHandler(two, left);
        collisionHandler(two, right);
        collisionHandler(two, top);
        collisionHandler(two, bot);
*/
        pWindow->drawFrame(frame->getBitmap_info(), frame->getPixels());

        std::this_thread::sleep_until(t1);

    }

    delete frame;

    return 0;
}