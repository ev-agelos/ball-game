#include <cmath>
#include <raylib.h>
#include <raymath.h>
#include "ball.h"
#include "player.h"
#include "bot.h"
#include "utils.h"
#include "sounds.h"

extern Rectangle FIELD;
extern Rectangle LEFT_NET;
extern Rectangle RIGHT_NET;
extern bool IS_GOAL;


Ball::Ball()
    :
    friction_c(0.3f),
    acceleration{0, 0},
    radius(5.f),
    position{0, 0},
    velocity{0, 0}
{
    setup();
}


void Ball::setup()
{
    position.x = ((FIELD.x + FIELD.width) / 2.f) + FIELD.x;
    position.y = FIELD.height / 2.f + FIELD.y;
    velocity = {0, 0};
}


void Ball::set_x(float val)
{
    last_position.x = position.x;
    // ball is in limits
    if (((val - radius) > FIELD.x) and ((val + radius) < FIELD.x + FIELD.width))
    {
        position.x = val;
        return;
    }

    // hit vertical wall else goal
    if (position.y < LEFT_NET.y or position.y > LEFT_NET.y + LEFT_NET.height)
        velocity.x *= -1;
    else
    {
        position.x = val;
        if (not IS_GOAL and (CheckCollisionPointRec(Vector2{position.x - radius, position.y}, LEFT_NET)
                             or CheckCollisionPointRec(Vector2{position.x + radius, position.y}, RIGHT_NET)))
            IS_GOAL = true;
    }
}


void Ball::set_y(float val)
{
    last_position.y = position.y;

    // ball is in limits
    if (((val - radius) > FIELD.y) && ((val + radius) < FIELD.y + FIELD.height))
    {
        position.y = val;
        return;
    }
    // hit horizontal wall or goal
    velocity.y *= -1;
}


void Ball::update(float dt)
{
    if (acceleration.x or acceleration.y)
    {
        velocity = Vector2Add(velocity, acceleration);
        acceleration = {0, 0};
    }
    else
    {
        velocity = Vector2Scale(velocity, pow(friction_c, dt));
        if (abs(velocity.x) < 0.1)
            velocity.x = 0;
        if (abs(velocity.y) < 0.1)
            velocity.y = 0;
    }

    set_x(position.x + velocity.x * dt);
    set_y(position.y + velocity.y * dt);
}


void Ball::roll(const Vector2& kick_direction, float power)
{
    Vector2 kick_velocity = Vector2Scale(kick_direction, power);
    acceleration = Vector2Subtract(kick_velocity, velocity);
}


void Ball::kick(const Vector2& kick_direction, float power)
{
    roll(kick_direction, power);
    play_ball_sound();
}


void Ball::handle_collision_response(const Player& p, const Vector2& velocity, float dt)
{
    if (Vector2DotProduct(p.input, Vector2Normalize(p.velocity)) < -0.99)
        return;
    position = last_position;
    this->velocity = velocity;
    set_x(position.x + this->velocity.x * dt);
    set_y(position.y + this->velocity.y * dt);

    if (abs(this->velocity.x) > p.max_speed or abs(this->velocity.y) > p.max_speed)
        this->velocity = Vector2Negate(this->velocity);
}
