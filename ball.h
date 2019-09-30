#pragma once
#include <raylib.h>

class Player;
class Bot;


class Ball {
private:
    const float acceleration_factor;
    const float friction_c;

public:
    Ball(Sound sound);
    float radius;
    Vector2 position;
    Vector2 acceleration;
    Vector2 friction;
    void set_x(float val);
    void set_y(float val);
    Vector2 velocity;
    Player *controlled_by;
    bool crossed_net;
    Sound kick_sound;
    void setup();
    void roll(const Vector2 &kick_direction, float power);
    void kick(const Vector2 &kick_direction, float power);
    void update();
    void check_collision(Player & p, Bot & bot);
    void apply_acceleration();
    void apply_friction();
};