#pragma once

#include <raylib.h>
#include "player.h"
#include "bot.h"


class Ball {
private:
    const float friction_c;
    Vector2 acceleration;
    void set_x(float val);
    void set_y(float val);
public:
    Ball();
    float radius;
    Vector2 position;
    Vector2 velocity;
    Vector2 last_position;
    void setup();
    void roll(const Vector2& kick_direction, float power);
    void kick(const Vector2& kick_direction, float power);
    void update(float dt);
    void handle_collision_response(const Player& p, const Vector2& velocity, float dt);
};
