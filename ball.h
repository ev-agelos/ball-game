#pragma once
#include <raylib.h>

class Player;
class Bot;


class Ball {
private:
    const float deceleration_factor;
public:
    Ball(Sound sound);
    float speed;
    float radius;
    Vector2 starting_position;
    Vector2 position;
    void set_x(float val);
    void set_y(float val);
    Vector2 velocity;
    Player* controlled_by;
    bool crossed_net;
    int left_score;
    int right_score;
    Sound kick_sound;
    void setup();
    void roll(const Player &p, float power);
    void kick(const Player &p, float power);
    void update();
    void check_collision(Player & p, Bot & bot);
};