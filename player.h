#pragma once
#include <raylib.h>

class Ball;


class Player
{
private:
    void handle_movement_control(Ball & ball);
protected:
    const float acceleration_factor;
    const float deceleration_factor;
    Color color;
    void update_pos(const Vector2 &v);
    void set_direction();
    void set_velocity();
    void set_valid_velocity(float &value);
public:
    float max_speed;
    Player();
    Rectangle rec;
    Vector2 direction;
    Vector2 velocity;
    int power;
    virtual void setup();
    void update(Ball & ball);
};