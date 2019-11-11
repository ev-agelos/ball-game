#pragma once

#include <raylib.h>

class Ball;


class Player
{
private:
    const float acceleration_factor;
    const float deceleration_factor;
    bool controls_ball;
    void update_pos(const Vector2& v);
    void read_user_input();
    void add_acceleration(float dt, const Vector2& ball_pos);
    void add_friction(float dt, const Vector2& ball_pos, float ball_radius);
    void set_velocity();
    const Vector2 get_kick_direction(const Vector2& ball_pos) const;
public:
    Vector2 acceleration;
    Vector2 input;
    Player(const float max_speed=200.f);
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
