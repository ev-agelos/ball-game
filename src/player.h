#pragma once
#include <raylib.h>

class Ball;


class Player
{
private:
    void handle_movement_control(float dt, Ball & ball);

protected:
    Vector2 acceleration;
    const float acceleration_factor;
    const float deceleration_factor;
    Color color;
    void update_pos(const Vector2 &v);
    void read_user_input();
    void apply_acceleration(float dt);
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
    void update(float dt, Ball & ball);
    bool is_velocity_constrained(Ball &ball, Vector2 new_velocity);
    void handle_collision_response(Ball &ball, Vector2 velocity, float dt);
    void kick(Ball &ball);
    Vector2 last_position;
};
