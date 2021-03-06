#include <iostream>

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
    if (e1.x - e2.x > e1.width || e2.x - e1.x > e2.width) return;
    if (e1.y - e2.y > e1.height || e2.y - e1.y > e2.height) return;

    std::cout << "Detected" << std::endl;

    // Interpolate for and update positions 

    if (e1.x < e2.x) {
        e1.x -= e1.velocity_x * (e1.x + e1.width - e2.x) / (e1.velocity_x - e2.velocity_x);
        e2.x -= e2.velocity_x * (e1.x + e1.width - e2.x) / (e1.velocity_x - e2.velocity_x);
    } else {
        e1.x -= e1.velocity_x * (e2.x + e2.width - e1.x) / (e2.velocity_x - e1.velocity_x);
        e2.x -= e2.velocity_x * (e2.x + e2.width - e1.x) / (e2.velocity_x - e1.velocity_x);
    }

    // Velocities

    int e1_iVelX = e1.velocity_x;
    int e2_iVelX = e2.velocity_x;
    e1.velocity_x = (e1.mass - e2.mass) / (e1.mass + e2.mass) * e1_iVelX + 2.0f * e2.mass / (e1.mass + e2.mass) * e2_iVelX;
    e2.velocity_x = 2.0f * e1.mass / (e1.mass + e2.mass) * e1_iVelX + (e2.mass - e1.mass) / (e1.mass + e2.mass) * e2_iVelX;

    int e1_iVelY = e1.velocity_y;
    int e2_iVelY = e2.velocity_y;
    e1.velocity_y = (e1.mass - e2.mass) / (e1.mass + e2.mass) * e1_iVelY + 2.0f * e2.mass / (e1.mass + e2.mass) * e2_iVelY;
    e2.velocity_y = 2.0f * e1.mass / (e1.mass + e2.mass) * e1_iVelY + (e2.mass - e1.mass) / (e1.mass + e2.mass) * e2_iVelY;
}