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
    if (e1.x - e2.x >= e2.width || e2.x - e1.x >= e1.width) return;
    if (e1.y - e2.y >= e2.height || e2.y - e1.y >= e1.height) return;

    // Interpolate for and update positions 

    if (e1.velocity_x - e2.velocity_x != 0) {
        float overlap = e1.x < e2.x ? (e1.x + e1.width - e2.x) : (e2.x + e2.width - e1.x);

        float frac_e1 = e1.velocity_x / (e1.x < e2.x ? (e1.velocity_x - e2.velocity_x) : (e2.velocity_x - e1.velocity_x) );
        float frac_e2 = e2.velocity_x / (e1.x < e2.x ? (e1.velocity_x - e2.velocity_x) : (e2.velocity_x - e1.velocity_x) );

        e1.x -= overlap * frac_e1;
        e2.x -= overlap * frac_e2;
    }

    if (e1.velocity_y - e2.velocity_y != 0) {
        float overlap = e1.y < e2.y ? (e1.y + e1.height - e2.y) : (e2.y + e2.height - e1.y);

        float frac_e1 = e1.velocity_y / (e1.y < e2.y ? (e1.velocity_y - e2.velocity_y) : (e2.velocity_y - e1.velocity_y) );
        float frac_e2 = e2.velocity_y / (e1.y < e2.y ? (e1.velocity_y - e2.velocity_y) : (e2.velocity_y - e1.velocity_y) );

        e1.y -= overlap * frac_e1;
        e2.y -= overlap * frac_e2;
    }

/*

    if (e1.x < e2.x) {
        e1.x -= e1.velocity_x * (e1.x + e1.width - e2.x) / (e1.velocity_x - e2.velocity_x);
        e2.x -= e2.velocity_x * (e1.x + e1.width - e2.x) / (e1.velocity_x - e2.velocity_x);
    } else {
        e1.x -= e1.velocity_x * (e2.x + e2.width - e1.x) / (e2.velocity_x - e1.velocity_x);
        e2.x -= e2.velocity_x * (e2.x + e2.width - e1.x) / (e2.velocity_x - e1.velocity_x);
    }

    if (e1.y < e2.y) {
        e1.y -= e1.velocity_y * (e1.y + e1.height - e2.y) / (e1.velocity_y - e2.velocity_y);
        e2.y -= e2.velocity_y * (e1.y + e1.height - e2.y) / (e1.velocity_y - e2.velocity_y);
    } else {
        e1.y -= e1.velocity_y * (e2.y + e2.height - e1.y) / (e2.velocity_y - e1.velocity_y);
        e2.y -= e2.velocity_y * (e2.y + e2.height - e1.y) / (e2.velocity_y - e1.velocity_y);
    }

*/

    // Velocities


    float iVel_e1X = e1.velocity_x;
    float iVel_e2X = e2.velocity_x;
    float iVel_e1Y = e1.velocity_y;
    float iVel_e2Y = e2.velocity_y;

    float mFrac1 = e1.mass / (e1.mass + e2.mass);
    float mFrac2 = e2.mass / (e1.mass + e2.mass);

    e1.velocity_x = (mFrac1 - mFrac2) * iVel_e1X + 2.0f * mFrac2 * iVel_e2X;
    e2.velocity_x = 2.0f * mFrac1 * iVel_e1X + (mFrac2 - mFrac1 ) * iVel_e2X;

    e1.velocity_y = (mFrac1 - mFrac2) * iVel_e1Y + 2.0f * mFrac2 * iVel_e2Y;
    e2.velocity_y = 2.0f * mFrac1 * iVel_e1Y + (mFrac2 - mFrac1 ) * iVel_e2Y;



/*
    float e1_iVelX = e1.velocity_x;
    float e1_iVelY = e1.velocity_y;

    float e2_iVelX = e2.velocity_x;
    float e2_iVelY = e2.velocity_y;

    float factor = 2.0f * ( (e1_iVelX - e2_iVelX) * (e1.x + e1.width / 2.0f - e2.x - e2.width / 2.0f ) + (e1_iVelY - e2_iVelY) * (e1.y + e1.height / 2.0f - e2.y - e2.height / 2.0f ));
    factor = factor / ( (e1.mass + e2.mass) * ((e1.x + e1.width / 2.0f - e2.x - e2.width / 2.0f ) * (e1.x + e1.width / 2.0f - e2.x - e2.width / 2 ) + (e1.y + e1.height / 2.0f - e2.y - e2.height / 2.0f ) * (e1.y + e1.height / 2.0f - e2.y - e2.height / 2.0f )) );

    e1.velocity_x = e1_iVelX - e2.mass * factor * (e1.x + e1.width / 2.0f - e2.x - e2.width / 2.0f);
    //e1.velocity_x = (e1.mass - e2.mass) / (e1.mass + e2.mass) * e1_iVelX + 2.0f * e2.mass / (e1.mass + e2.mass) * e2_iVelX;
    e1.velocity_y = e1_iVelY - e2.mass * factor * (e1.y + e1.height / 2.0f - e2.y - e2.height / 2.0f);
    //e1.velocity_y = (e1.mass - e2.mass) / (e1.mass + e2.mass) * e1_iVelY + 2.0f * e2.mass / (e1.mass + e2.mass) * e2_iVelY;

    e2.velocity_x = e2_iVelX - e1.mass * factor * (e2.x - e2.width / 2.0f - e1.x + e1.width / 2.0f);
    //e2.velocity_x = 2.0f * e1.mass / (e1.mass + e2.mass) * e1_iVelX + (e2.mass - e1.mass) / (e1.mass + e2.mass) * e2_iVelX;
    e2.velocity_y = e2_iVelY - e1.mass * factor * (e2.y - e2.height / 2.0f - e1.y + e1.height / 2.0f);
    //e2.velocity_y = 2.0f * e1.mass / (e1.mass + e2.mass) * e1_iVelY + (e2.mass - e1.mass) / (e1.mass + e2.mass) * e2_iVelY;
*/
}