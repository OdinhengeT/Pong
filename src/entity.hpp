#ifndef ENTITIES_H
#define ENTITIES_H

class Entity {
    private:
        float x, y;
        float width, height;
        float mass;
        float velocity_x, velocity_y;
        float rotation;
    public:
        Entity(float x, float y, float width, float height, float mass = 1.0f);
        Entity(const Entity&)=default;
        ~Entity()=default;

        float getPositionX() const;
        float getPositionY() const;
        float getVelocityX() const;
        float getVelocityY() const;
        float getRotation() const;
        float getWidth() const;
        float getHeight() const;

        void setPositionX(int x);
        void setPositionY(int y);
        void setVelocityX(float velocity_x);
        void setVelocityY(float velocity_y);
        void setRotation(float rotation);

        void timeStep(float dt);

        friend void collisionHandler(Entity& e1, Entity& e2);
};

//void collisionHandler(Entity& e1, Entity& e2);

#endif