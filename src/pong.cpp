#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include "entity/entity.hpp"
#include "graphics/frame.hpp"
#include "window/window.hpp"


static const int WIDTH = 1280;
static const int HEIGHT = 720;
static const int MARGIN = 12;
static const int FONT_SIDE_LENGTH = 8;
static const int FONT_SCALE = 10;
static const int OFFSET_Y = 3 * MARGIN + FONT_SIDE_LENGTH * FONT_SCALE;

static const float TIME_STEP = 1.0f / 60.0f; // 1 / fps (delta time in seconds)

void simulatePhysics(Entity& ball, Entity& playerLeft, Entity& playerRight) {    
    // Update Velocities

    playerLeft.setVelocityY(((GetKeyState('S') & 0x8000) - (GetKeyState('W') & 0x8000)) / 0x8000 * 25000 * TIME_STEP);
    if (playerLeft.getPositionY() <= OFFSET_Y && playerLeft.getVelocityY() < 0) {
        playerLeft.setPositionY(OFFSET_Y);
        playerLeft.setVelocityY(0);
    } else if (playerLeft.getPositionY() >= HEIGHT - 2 * MARGIN - playerLeft.getHeight() && playerLeft.getVelocityY() > 0) {
        playerLeft.setPositionY(HEIGHT - 2 * MARGIN - playerLeft.getHeight());
        playerLeft.setVelocityY(0);
    }

    playerRight.setVelocityY(((GetKeyState(VK_DOWN) & 0x8000) - (GetKeyState(VK_UP) & 0x8000)) / 0x8000 * 500);
    if (playerRight.getPositionY() <= OFFSET_Y && playerRight.getVelocityY() < 0) {
        playerRight.setPositionY(OFFSET_Y);
        playerRight.setVelocityY(0);
    } else if (playerRight.getPositionY() >= HEIGHT - 2 * MARGIN - playerRight.getHeight() && playerRight.getVelocityY() > 0) {
        playerRight.setPositionY(HEIGHT - 2 * MARGIN - playerRight.getHeight());
        playerRight.setVelocityY(0);
    }

    // Translate objects

    ball.timeStep(TIME_STEP);
    playerLeft.timeStep(TIME_STEP);
    playerRight.timeStep(TIME_STEP);

    // Handle collisions

    collisionHandler(playerLeft, ball);
    collisionHandler(playerRight, ball);
    
}

void drawFrame(Frame* frame, int score_left, int score_right, const Entity& ball, const Entity& playerLeft, const Entity& playerRight) {
    // Clear
    frame->clear();
    
    // Drawing Scoreboard
    if (score_left < 10) {
        frame->drawString(WIDTH / 2 - 7 * FONT_SIDE_LENGTH * FONT_SCALE, MARGIN, "0", FONT_SCALE, FONT_SCALE, makeRGB(255, 255, 255));
        frame->drawString(WIDTH / 2 - 6 * FONT_SIDE_LENGTH * FONT_SCALE, MARGIN, std::to_string(score_left), FONT_SCALE, FONT_SCALE, makeRGB(255, 255, 255));
    } else {
        frame->drawString(WIDTH / 2 - 7 * FONT_SIDE_LENGTH * FONT_SCALE, MARGIN, std::to_string(score_left), FONT_SCALE, FONT_SCALE, makeRGB(255, 255, 255));
    }

    frame->drawString(WIDTH / 2 - 3 * FONT_SIDE_LENGTH * FONT_SCALE, MARGIN, "=PONG=", FONT_SCALE, FONT_SCALE, makeRGB(255, 255, 255));

    if (score_right < 10) {
        frame->drawString(WIDTH / 2 + 5 * FONT_SIDE_LENGTH * FONT_SCALE, MARGIN, "0", FONT_SCALE, FONT_SCALE, makeRGB(255, 255, 255));
        frame->drawString(WIDTH / 2 + 6 * FONT_SIDE_LENGTH * FONT_SCALE, MARGIN, std::to_string(score_right), FONT_SCALE, FONT_SCALE, makeRGB(255, 255, 255));
    } else {
        frame->drawString(WIDTH / 2 + 5 * FONT_SIDE_LENGTH * FONT_SCALE, MARGIN, std::to_string(score_right), FONT_SCALE, FONT_SCALE, makeRGB(255, 255, 255));
    }

    // Playing Field
    frame->drawRectangle( // Top Boder
        MARGIN, OFFSET_Y - MARGIN, WIDTH - 2 * MARGIN, MARGIN, 
        makeRGB(255, 255, 255)
    );
    frame->drawRectangle( // Bottom Border
        MARGIN, HEIGHT - 2 * MARGIN, WIDTH - 2 * MARGIN, MARGIN, 
        makeRGB(255, 255, 255)
    );

    // Middle divide
    frame->drawRectangle( // White Line
        (WIDTH - MARGIN) / 2, OFFSET_Y, MARGIN, HEIGHT - OFFSET_Y - 2 * MARGIN, 
        makeRGB(255, 255, 255)
    );

    int nbr_dash = (HEIGHT - OFFSET_Y - 2 * MARGIN) / MARGIN;
    if (nbr_dash % 2 == 0) nbr_dash--;

    int dash_offset = (HEIGHT - OFFSET_Y - (nbr_dash + 2) * MARGIN) / 2;

    for (int i = 0; i < nbr_dash; i = i + 2) {  // Black Lines 
        frame->drawRectangle(
            (WIDTH - MARGIN) / 2, OFFSET_Y + dash_offset + i * MARGIN, MARGIN, MARGIN, 
            makeRGB(0, 0, 0)
        );
    }

    // Players & Ball

    frame->drawRectangle(
        static_cast<int>(ball.getPositionX() + 0.5f), 
        static_cast<int>(ball.getPositionY() + 0.5f), 
        static_cast<int>(ball.getWidth() + 0.5f), 
        static_cast<int>(ball.getHeight()) + 0.5f, 
        makeRGB(255, 255, 255)
    );

    frame->drawRectangle(
        static_cast<int>(playerLeft.getPositionX() + 0.5f), 
        static_cast<int>(playerLeft.getPositionY() + 0.5f), 
        static_cast<int>(playerLeft.getWidth() + 0.5f), 
        static_cast<int>(playerLeft.getHeight() + 0.5f), 
        makeRGB(255, 255, 255)
    );
    
    frame->drawRectangle(
        static_cast<int>(playerRight.getPositionX() + 0.5f), 
        static_cast<int>(playerRight.getPositionY() + 0.5f), 
        static_cast<int>(playerRight.getWidth() + 0.5f), 
        static_cast<int>(playerRight.getHeight() + 0.5f), 
        makeRGB(255, 255, 255)
    );

}

int main() {
    std::cout << "Creating Window" << std::endl;

    auto pWindow = std::unique_ptr<Window>(new Window(L"Pong", WIDTH, HEIGHT));

    Frame* frame = new Frame(WIDTH, HEIGHT);
    Frame* frame2 = new Frame(WIDTH, HEIGHT);
    
    int score_left = 4, score_right = 12;

    // Moving Things

    Entity ball(WIDTH / 2 - MARGIN, (HEIGHT + OFFSET_Y) / 2 - 2 * MARGIN, 2 * MARGIN, 2 * MARGIN);
    ball.setVelocityX(500.0f);
    //ball.setVelocityY();

    Entity playerLeft(MARGIN, (HEIGHT + OFFSET_Y) / 2 - 5 * MARGIN, MARGIN, 10 * MARGIN, 10000.0f);
   
    Entity playerRight(WIDTH - 2 * MARGIN, (HEIGHT + OFFSET_Y) / 2 - 5 * MARGIN, MARGIN, 10 * MARGIN, 10000.0f);

    bool running = true; 
    while (running) {

        auto t0 = std::chrono::high_resolution_clock::now();
        auto t1 = t0 + std::chrono::microseconds(static_cast<int>(1000000.0f * TIME_STEP));

        if (!pWindow->MsgProc()) {
            std::cout << "Closing Window" << std::endl;
            running = false;
        }
        
        // Physics

        simulatePhysics(ball, playerLeft, playerRight);

        // Drawing

        drawFrame(frame, score_left, score_right, ball, playerLeft, playerRight);
        pWindow->drawFrame(frame->getBitmap_info(), frame->getPixels());

        std::this_thread::sleep_until(t1);
    }

    delete frame;
    delete frame2; 

    return 0;
}