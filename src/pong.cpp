#include <iostream>
#include <chrono>
#include <thread>
#include <memory>

#include "window/window.hpp"
#include "graphics/frame.hpp"
#include "qShapes/qShapes.hpp"
#include "fonts/ThorFont.hpp"

static const int WIDTH = 1280;
static const int HEIGHT = 720;
static const int MARGIN = 12;
static const int FONT_SIDE_LENGTH = 8;
static const int FONT_SCALE = 10;
static const int OFFSET_Y = 3 * MARGIN + FONT_SIDE_LENGTH * FONT_SCALE;

static const int TIME_STEP = 1000 / 60; // 1000 / fps (delta time in milliseconds)

void simulatePhysics(
    qShapes::qRectangle2D& ball_position, 
    qShapes::qPoint2D& ball_velocity,
    qShapes::qRectangle2D& player_left_position, 
    qShapes::qPoint2D& player_left_velocity,
    qShapes::qRectangle2D& player_right_position,
    qShapes::qPoint2D& player_right_velocity
) {    
    // Update Velocities

    player_left_velocity.y = ((GetKeyState('S') & 0x8000) - (GetKeyState('W') & 0x8000)) * TIME_STEP / 0x10000;
    if (player_left_position.vert_1.y <= OFFSET_Y && player_left_velocity.y < 0) {
        player_left_position.vert_1.y = OFFSET_Y;
        player_left_velocity.y = 0;
    } else if (player_left_position.vert_1.y >= HEIGHT - 2 * MARGIN - player_left_position.height && player_left_velocity.y > 0) {
        player_left_position.vert_1.y = HEIGHT - 2 * MARGIN - player_left_position.height;
        player_left_velocity.y = 0;
    }

    player_right_velocity.y = ((GetKeyState(VK_DOWN) & 0x8000) - (GetKeyState(VK_UP) & 0x8000)) * TIME_STEP / 0x10000;
    if (player_right_position.vert_1.y <= OFFSET_Y && player_right_velocity.y < 0) {
        player_right_position.vert_1.y = OFFSET_Y;
        player_right_velocity.y = 0;
    } else if (player_right_position.vert_1.y >= HEIGHT - 2 * MARGIN - player_right_position.height && player_right_velocity.y > 0) {
        player_right_position.vert_1.y = HEIGHT - 2 * MARGIN - player_right_position.height;
        player_right_velocity.y = 0;
    }

    // Translate objects

    ball_position.translate(ball_velocity);
    player_left_position.translate(player_left_velocity);
    player_right_position.translate(player_right_velocity);

    // Handle collisions

    if (ball_position.vert_1.y <= OFFSET_Y) {
        ball_position.vert_1.y += 2 * (OFFSET_Y - ball_position.vert_1.y);
        ball_velocity.y *= -1;
    } else if (ball_position.vert_1.y >= HEIGHT - 2 * MARGIN - ball_position.height) {
        ball_position.vert_1.y -= 2 * (ball_position.vert_1.y - HEIGHT + 2 * MARGIN + ball_position.height);
        ball_velocity.y *= -1;
    }

    if (ball_position.vert_1.x <= 2 * MARGIN) {
        
        ball_velocity.x *= -1;
    } else if (ball_position.vert_1.x >= WIDTH - 2 * MARGIN - ball_position.width) {
        ball_velocity.x *= -1;
    }
}

void drawFrame(
    Frame* frame, 
    int score_left, 
    int score_right,
    const qShapes::qRectangle2D& ball_position,
    const qShapes::qRectangle2D& player_left_position,
    const qShapes::qRectangle2D& player_right_position
) {
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
        qShapes::qRectangle2D(MARGIN, OFFSET_Y - MARGIN, WIDTH - 2 * MARGIN, MARGIN), 
        makeRGB(255, 255, 255)
    );
    frame->drawRectangle( // Bottom Border
        qShapes::qRectangle2D(MARGIN, HEIGHT - 2 * MARGIN, WIDTH - 2 * MARGIN, MARGIN), 
        makeRGB(255, 255, 255)
    );

    // Middle divide
    frame->drawRectangle( // White Line
        qShapes::qRectangle2D((WIDTH - MARGIN) / 2, OFFSET_Y, MARGIN, HEIGHT - OFFSET_Y - 2 * MARGIN), 
        makeRGB(255, 255, 255)
    );

    int nbr_dash = (HEIGHT - OFFSET_Y - 2 * MARGIN) / MARGIN;
    if (nbr_dash % 2 == 0) nbr_dash--;

    int dash_offset = (HEIGHT - OFFSET_Y - (nbr_dash + 2) * MARGIN) / 2;

    for (int i = 0; i < nbr_dash; i = i + 2) {  // Black Lines 
        frame->drawRectangle(
            qShapes::qRectangle2D((WIDTH - MARGIN) / 2, OFFSET_Y + dash_offset + i * MARGIN, MARGIN, MARGIN), 
            makeRGB(0, 0, 0)
        );
    }

    // Players & Ball

    frame->drawRectangle(ball_position, makeRGB(255, 255, 255));
    frame->drawRectangle(player_left_position, makeRGB(255, 255, 255));
    frame->drawRectangle(player_right_position, makeRGB(255, 255, 255));

}

int main() {
    std::cout << "Creating Window" << std::endl;

    auto pWindow = std::unique_ptr<Window>(new Window(L"Pong", WIDTH, HEIGHT));

    Frame* frame = new Frame(WIDTH, HEIGHT);
    Frame* frame2 = new Frame(WIDTH, HEIGHT);
    
    int score_left = 4, score_right = 12;

    // Moving Things

    qShapes::qRectangle2D ball_position(
        WIDTH / 2 - MARGIN, 
        (HEIGHT + OFFSET_Y) / 2 - 2 * MARGIN, 
        2 * MARGIN, 
        2 * MARGIN
    );
    qShapes::qPoint2D ball_velocity(TIME_STEP / 3,TIME_STEP / 2);

    qShapes::qRectangle2D player_left_position(
        MARGIN, 
        (HEIGHT + OFFSET_Y) / 2 - 5 * MARGIN, 
        MARGIN, 
        10 * MARGIN
    );
    qShapes::qPoint2D player_left_velocity(0, 0);

    qShapes::qRectangle2D player_right_position(
        WIDTH - 2 * MARGIN, 
        (HEIGHT + OFFSET_Y) / 2 - 5 * MARGIN, 
        MARGIN, 
        10 * MARGIN
    );
    qShapes::qPoint2D player_right_velocity(0, 0);

    bool running = true; 
    while (running) {

        auto t0 = std::chrono::high_resolution_clock::now();
        auto t1 = t0 + std::chrono::milliseconds(TIME_STEP);

        if (!pWindow->MsgProc()) {
            std::cout << "Closing Window" << std::endl;
            running = false;
        }
        
        // Physics

        simulatePhysics(ball_position, ball_velocity, player_left_position, player_left_velocity, player_right_position, player_right_velocity);

        // Drawing

        drawFrame(frame, score_left, score_right, ball_position, player_left_position, player_right_position);
        pWindow->drawFrame(frame->getBitmap_info(), frame->getPixels());

        std::this_thread::sleep_until(t1);
    }

    delete frame;
    delete frame2; 

    return 0;
}