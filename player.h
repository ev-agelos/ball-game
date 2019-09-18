#pragma once
#include <raylib.h>

class Ball;


class Player
{
private:
    void handle_movement_control(Ball & ball);
    bool ball_collision;

protected:
    Vector2 acceleration;
    const float acceleration_factor;
    const float deceleration_factor;
    Color color;
    void update_pos(const Vector2 &v);
    void set_acceleration();
    void set_velocity();
    const Vector2 get_kick_direction(const Vector2 &ball_pos) const;
public:
    float max_speed;
    Player();
    Vector2 position;
    Vector2 size;
    Vector2 direction;
    Vector2 velocity;
    int power;
    virtual void setup();
    void update(Ball & ball);
};