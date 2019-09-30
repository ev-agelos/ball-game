#pragma once
#include <raylib.h>

class Ball;


class Player
{
private:
    void handle_movement_control(Ball & ball);
    bool ball_inside_rectangle;

protected:
    Vector2 acceleration;
    const float acceleration_factor;
    const float deceleration_factor;
    Color color;
    void update_pos(const Vector2 &v);
    void read_user_input();
    void apply_acceleration();
    void set_velocity();
    const Vector2 get_kick_direction(const Vector2 &ball_pos) const;
public:
    float max_speed;
    Player();
    Rectangle rec;
    Vector2 input;
    Vector2 velocity;
    int power;
    virtual void setup();
    void update(Ball & ball);
    bool is_velocity_constrained(Ball &ball, Vector2 new_velocity);
    void kick(Ball &ball);
};