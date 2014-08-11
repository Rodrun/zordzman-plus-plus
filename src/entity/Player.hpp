#pragma once

#include "Entity.hpp"

class Player : public Entity {
public:
    Player(float x, float y, float speed);
    void render();
    void tick();
    Player *clone() const;

private:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    float m_speed;
};