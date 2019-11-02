#pragma once

#include <raylib.h>

class Ball;


class Player
{
private:
    Vector2 input;
    Vector2 acceleration;
    const float acceleration_factor;
    const float deceleration_factor;
    Color color;
    void update_pos(const Vector2& v);
    void read_user_input();
    void apply_acceleration(float& dt);
    void set_velocity();
    void handle_movement_control(float& dt, Ball& ball);
    const Vector2 get_kick_direction(const Vector2& ball_pos) const;
public:
    Player(const float max_speed=100.f);
    const float max_speed;
    Rectangle rec;
    Vector2 velocity;
    int power;
    Vector2 last_position;
    const int max_power = 1000;
    virtual void setup();
    void update(float& dt, Ball& ball);
    void handle_collision_response(Ball& ball, const Vector2& velocity, float& dt);
    void kick(Ball& ball);
};
