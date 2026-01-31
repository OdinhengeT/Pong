#include "entity.hpp"

Entity::Entity(float x, float y, float width, float height, float mass):
    x{x}, y{y},
    width{width}, height{height},
    mass{mass},
    velocity_x{0}, velocity_y{0},
    rotation{0}
{}

float Entity::getPositionX() const {
    return this->x;
}

float Entity::getPositionY() const {
    return this->y;
}

float Entity::getVelocityX() const {
    return this->velocity_x;
}

float Entity::getVelocityY() const {
    return this->velocity_y;
}

float Entity::getRotation() const {
    return this->rotation;
}

float Entity::getWidth() const {
    return this->width;
}

float Entity::getHeight() const {
    return this->height;
}

void Entity::setPositionX(int x) {
    this->x = x;
}

void Entity::setPositionY(int y) {
    this->y = y;
}

void Entity::setVelocityX(float velocity_x) {
    this->velocity_x = velocity_x;
}

void Entity::setVelocityY(float velocity_y) {
    this->velocity_y = velocity_y;
}

void Entity::setRotation(float rotation) {
    this->rotation = rotation;
}

void Entity::timeStep(float dt) {
    this->x += this->velocity_x * dt;
    this->y += this->velocity_y * dt;
}

void collisionHandler(Entity& e1, Entity& e2) {
    // Check for collision
    if (e1.x - e2.x >= e2.width || e2.x - e1.x >= e1.width) return;
    if (e1.y - e2.y >= e2.height || e2.y - e1.y >= e1.height) return;

    // Calculate overlap amounts
    float overlap_x = e1.x < e2.x ? (e1.x + e1.width - e2.x) : (e2.x + e2.width - e1.x);
    float overlap_y = e1.y < e2.y ? (e1.y + e1.height - e2.y) : (e2.y + e2.height - e1.y);

    // Determine collision axis (smallest overlap)
    // For Pong, we typically want X-axis collisions (paddle hits)
    if (overlap_x < overlap_y) {
        // X-axis collision (paddle collision)
        // Separate the objects
        if (e1.x < e2.x) {
            // e1 is on the left
            e1.x -= overlap_x / 2;
            e2.x += overlap_x / 2;
        } else {
            // e1 is on the right
            e1.x += overlap_x / 2;
            e2.x -= overlap_x / 2;
        }

        // For Pong: reverse the ball's X velocity, keep paddle stationary
        // If e1 is the paddle (very heavy), only reverse e2's velocity
        if (e1.mass > e2.mass * 100) {
            // e1 is paddle, e2 is ball
            e2.velocity_x = -e2.velocity_x;
            // Optionally add some of the paddle's Y velocity to the ball
            e2.velocity_y += e1.velocity_y * 0.3f;
        } else if (e2.mass > e1.mass * 100) {
            // e2 is paddle, e1 is ball
            e1.velocity_x = -e1.velocity_x;
            // Optionally add some of the paddle's Y velocity to the ball
            e1.velocity_y += e2.velocity_y * 0.3f;
        } else {
            // Both objects are similar mass - elastic collision
            float v1 = e1.velocity_x;
            float v2 = e2.velocity_x;
            e1.velocity_x = ((e1.mass - e2.mass) * v1 + 2 * e2.mass * v2) / (e1.mass + e2.mass);
            e2.velocity_x = ((e2.mass - e1.mass) * v2 + 2 * e1.mass * v1) / (e1.mass + e2.mass);
        }
    } else {
        // Y-axis collision (top/bottom wall or unusual collision)
        // Separate the objects
        if (e1.y < e2.y) {
            // e1 is above
            e1.y -= overlap_y / 2;
            e2.y += overlap_y / 2;
        } else {
            // e1 is below
            e1.y += overlap_y / 2;
            e2.y -= overlap_y / 2;
        }

        // Reverse Y velocities
        if (e1.mass > e2.mass * 100) {
            e2.velocity_y = -e2.velocity_y;
        } else if (e2.mass > e1.mass * 100) {
            e1.velocity_y = -e1.velocity_y;
        } else {
            float v1 = e1.velocity_y;
            float v2 = e2.velocity_y;
            e1.velocity_y = ((e1.mass - e2.mass) * v1 + 2 * e2.mass * v2) / (e1.mass + e2.mass);
            e2.velocity_y = ((e2.mass - e1.mass) * v2 + 2 * e1.mass * v1) / (e1.mass + e2.mass);
        }
    }
}
