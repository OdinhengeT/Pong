#define SDL_MAIN_USE_CALLBACKS 1
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

#include "entity/entity.hpp"
#include "graphics/frame.hpp"

// Constants
static const int WIDTH = 1280;
static const int HEIGHT = 720;
static const int MARGIN = 12;
static const int FONT_SIDE_LENGTH = 8;
static const int FONT_SCALE = 10;
static const int OFFSET_Y = 3 * MARGIN + FONT_SIDE_LENGTH * FONT_SCALE;
static const float TIME_STEP = 1.0f / 60.0f; // 60 FPS

// Game state structure
struct GameState {
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    Frame* frame;

    Entity* ball;
    Entity* playerLeft;
    Entity* playerRight;

    int score_left;
    int score_right;

    Uint64 last_time;
};

void simulatePhysics(Entity& ball, Entity& playerLeft, Entity& playerRight) {
    // Get keyboard state
    const bool* keystate = SDL_GetKeyboardState(nullptr);

    // Update left player velocity (W/S keys)
    float leftVel = 0;
    if (keystate[SDL_SCANCODE_S]) leftVel += 500;
    if (keystate[SDL_SCANCODE_W]) leftVel -= 500;

    playerLeft.setVelocityY(leftVel);

    // Clamp left player position
    if (playerLeft.getPositionY() <= OFFSET_Y && playerLeft.getVelocityY() < 0) {
        playerLeft.setPositionY(OFFSET_Y);
        playerLeft.setVelocityY(0);
    } else if (playerLeft.getPositionY() >= HEIGHT - 2 * MARGIN - playerLeft.getHeight() && playerLeft.getVelocityY() > 0) {
        playerLeft.setPositionY(HEIGHT - 2 * MARGIN - playerLeft.getHeight());
        playerLeft.setVelocityY(0);
    }

    // Update right player velocity (Arrow keys)
    float rightVel = 0;
    if (keystate[SDL_SCANCODE_DOWN]) rightVel += 500;
    if (keystate[SDL_SCANCODE_UP]) rightVel -= 500;

    playerRight.setVelocityY(rightVel);

    // Clamp right player position
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

    // Ball collision with top/bottom walls
    if (ball.getPositionY() <= OFFSET_Y) {
        ball.setPositionY(OFFSET_Y);
        ball.setVelocityY(-ball.getVelocityY());
    } else if (ball.getPositionY() + ball.getHeight() >= HEIGHT - 2 * MARGIN) {
        ball.setPositionY(HEIGHT - 2 * MARGIN - ball.getHeight());
        ball.setVelocityY(-ball.getVelocityY());
    }

    // Handle collisions with paddles
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

    // Playing Field - Top Border
    frame->drawRectangle(
        MARGIN, OFFSET_Y - MARGIN, WIDTH - 2 * MARGIN, MARGIN,
        makeRGB(255, 255, 255)
    );

    // Bottom Border
    frame->drawRectangle(
        MARGIN, HEIGHT - 2 * MARGIN, WIDTH - 2 * MARGIN, MARGIN,
        makeRGB(255, 255, 255)
    );

    // Middle divide - White Line
    frame->drawRectangle(
        (WIDTH - MARGIN) / 2, OFFSET_Y, MARGIN, HEIGHT - OFFSET_Y - 2 * MARGIN,
        makeRGB(255, 255, 255)
    );

    // Middle divide - Black dashed lines
    int nbr_dash = (HEIGHT - OFFSET_Y - 2 * MARGIN) / MARGIN;
    if (nbr_dash % 2 == 0) nbr_dash--;
    int dash_offset = (HEIGHT - OFFSET_Y - (nbr_dash + 2) * MARGIN) / 2;

    for (int i = 0; i < nbr_dash; i += 2) {
        frame->drawRectangle(
            (WIDTH - MARGIN) / 2, OFFSET_Y + dash_offset + i * MARGIN, MARGIN, MARGIN,
            makeRGB(0, 0, 0)
        );
    }

    // Draw Ball
    frame->drawRectangle(
        static_cast<int>(ball.getPositionX() + 0.5f),
        static_cast<int>(ball.getPositionY() + 0.5f),
        static_cast<int>(ball.getWidth() + 0.5f),
        static_cast<int>(ball.getHeight() + 0.5f),
        makeRGB(255, 255, 255)
    );

    // Draw Left Player
    frame->drawRectangle(
        static_cast<int>(playerLeft.getPositionX() + 0.5f),
        static_cast<int>(playerLeft.getPositionY() + 0.5f),
        static_cast<int>(playerLeft.getWidth() + 0.5f),
        static_cast<int>(playerLeft.getHeight() + 0.5f),
        makeRGB(255, 255, 255)
    );

    // Draw Right Player
    frame->drawRectangle(
        static_cast<int>(playerRight.getPositionX() + 0.5f),
        static_cast<int>(playerRight.getPositionY() + 0.5f),
        static_cast<int>(playerRight.getWidth() + 0.5f),
        static_cast<int>(playerRight.getHeight() + 0.5f),
        makeRGB(255, 255, 255)
    );
}

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
    SDL_SetAppMetadata("Pong", "1.0", "com.odinhenget.pong");

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // Create game state
    GameState* state = new GameState();
    *appstate = state;

    if (!SDL_CreateWindowAndRenderer("Pong", WIDTH, HEIGHT, 0, &state->window, &state->renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        delete state;
        return SDL_APP_FAILURE;
    }

    // Create texture for rendering the frame buffer
    state->texture = SDL_CreateTexture(
        state->renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (!state->texture) {
        SDL_Log("Couldn't create texture: %s", SDL_GetError());
        delete state;
        return SDL_APP_FAILURE;
    }

    // Initialize frame buffer
    state->frame = new Frame(WIDTH, HEIGHT);

    // Initialize game objects
    state->ball = new Entity(WIDTH / 2.0 - MARGIN, (HEIGHT + OFFSET_Y) / 2.0 - 2 * MARGIN, 2 * MARGIN, 2 * MARGIN);
    state->ball->setVelocityX(500.0f);

    state->playerLeft = new Entity(MARGIN, (HEIGHT + OFFSET_Y) / 2.0 - 5 * MARGIN, MARGIN, 10 * MARGIN, 10000.0f);
    state->playerRight = new Entity(WIDTH - 2 * MARGIN, (HEIGHT + OFFSET_Y) / 2.0 - 5 * MARGIN, MARGIN, 10 * MARGIN, 10000.0f);

    state->score_left = 0;
    state->score_right = 0;

    state->last_time = SDL_GetTicks();

    std::cout << "Pong initialized successfully" << std::endl;

    return SDL_APP_CONTINUE;
}

/* This function runs when a new event occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_SUCCESS;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once per frame. */
SDL_AppResult SDL_AppIterate(void *appstate) {
    GameState* state = static_cast<GameState*>(appstate);

    // Check if ball went off screen (scoring)
    if (state->ball->getPositionX() < 0) {
        // Right player scores
        state->score_right++;
        // Reset ball to center
        state->ball->setPositionX(WIDTH / 2 - MARGIN);
        state->ball->setPositionY((HEIGHT + OFFSET_Y) / 2 - 2 * MARGIN);
        state->ball->setVelocityX(500.0f); // Ball goes right
        state->ball->setVelocityY(0);
    } else if (state->ball->getPositionX() > WIDTH) {
        // Left player scores
        state->score_left++;
        // Reset ball to center
        state->ball->setPositionX(WIDTH / 2 - MARGIN);
        state->ball->setPositionY((HEIGHT + OFFSET_Y) / 2 - 2 * MARGIN);
        state->ball->setVelocityX(-500.0f); // Ball goes left
        state->ball->setVelocityY(0);
    }

    // Fixed time step - run physics at 60 FPS
    simulatePhysics(*state->ball, *state->playerLeft, *state->playerRight);

    // Draw the frame
    drawFrame(state->frame, state->score_left, state->score_right,
              *state->ball, *state->playerLeft, *state->playerRight);

    // Render frame buffer to texture
    state->frame->renderToTexture(state->renderer, state->texture);

    // Clear and render
    SDL_SetRenderDrawColor(state->renderer, 0, 0, 0, 255);
    SDL_RenderClear(state->renderer);
    SDL_RenderTexture(state->renderer, state->texture, nullptr, nullptr);
    SDL_RenderPresent(state->renderer);

    // Maintain 60 FPS
    SDL_Delay(static_cast<Uint32>(TIME_STEP * 1000));

    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    GameState* state = static_cast<GameState*>(appstate);

    if (state) {
        std::cout << "Shutting down Pong" << std::endl;

        delete state->ball;
        delete state->playerLeft;
        delete state->playerRight;
        delete state->frame;

        if (state->texture) {
            SDL_DestroyTexture(state->texture);
        }

        delete state;
    }
}
