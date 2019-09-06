#pragma once
#include <raylib.h>

class Player;
class Bot;


class Ball {
private:
    const float deceleration_factor;
public:
    Ball();
    float speed;
    float radius;
    Vector2 starting_position;
    Vector2 position;
    Vector2 direction;
    void set_x(float val);
    void set_y(float val);
    Vector2 velocity;
    Player* controlled_by;
    void roll(const Player & p, float power);
    void update();
    void check_collision(Player & p, Bot & bot);
    Sound kick_sound;
};