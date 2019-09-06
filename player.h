#pragma once
#include <raylib.h>

class Ball;


class Player
{
private:
    void handle_movement_control(Ball & ball);
protected:
    float max_speed;
    const float acceleration_factor;
    const float deceleration_factor;
    Color color;
    void update_pos(const Vector2 &v);
    void set_direction();
    void set_velocity();
    void set_valid_velocity(float &value);
public:
    Player(float x, float y);
    Rectangle rec;
    Vector2 direction;
    Vector2 velocity;
    int power;
    void update(Ball & ball);
};