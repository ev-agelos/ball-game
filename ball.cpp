#include <iostream>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include "ball.h"
#include "player.h"
#include "bot.h"
#include "utils.h"

extern const int TOP_BOUND;
extern const int BOTTOM_BOUND;
extern const int LEFT_BOUND;
extern const int RIGHT_BOUND;
extern const int GOALPOST_THICKNESS;
extern const float GOALPOST_HEIGHT_START;
extern const float GOALPOST_HEIGHT_END;
extern int LEFT_SCORE;
extern int RIGHT_SCORE;


Ball::Ball(Sound sound)
    :
    friction_c(0.02),
    radius(5.f),
    position{0, 0},
    acceleration{0, 0},
    friction{0, 0},
    velocity{0, 0},
    controlled_by(nullptr),
    crossed_net(false),
    kick_sound(sound)
{
}


void Ball::setup()
{
    position.x = ((RIGHT_BOUND - LEFT_BOUND) / 2.f) + LEFT_BOUND;
    position.y = ((BOTTOM_BOUND - TOP_BOUND) / 2.f) + TOP_BOUND;
    velocity = {0, 0};
    crossed_net = false;
    controlled_by = nullptr;
}


void Ball::set_x(float val)
{
    // ball is in limits
    if (((val - radius) > LEFT_BOUND) && ((val + radius) < RIGHT_BOUND))
    {
        position.x = val;
        return;
    }

    // hit vertical wall else goal
    if (position.y < (GOALPOST_HEIGHT_START + GOALPOST_THICKNESS) || position.y > GOALPOST_HEIGHT_END)
        velocity.x *= -1;
    else
    {
        position.x = val;

        if (!crossed_net)
        {
            if (position.x < LEFT_BOUND)
            {
                RIGHT_SCORE += 1;
                crossed_net = true;
            }
            else if (position.x > RIGHT_BOUND)
            {
                LEFT_SCORE += 1;
                crossed_net = true;
            }
        }
    }

    controlled_by = nullptr;
}


void Ball::set_y(float val)
{
    // ball is in limits
    if (((val - radius) > TOP_BOUND) && ((val + radius) < BOTTOM_BOUND))
    {
        position.y = val;
        return;
    }
    // hit horizontal wall or goal
    velocity.y *= -1;
    controlled_by = nullptr;
}


void Ball::apply_acceleration()
{
    velocity = Vector2Add(velocity, acceleration);
    acceleration = {0, 0};
}


void Ball::apply_friction()
{
    if (!velocity.x and !velocity.y)
        return;

    Vector2 norm_velocity = Vector2Normalize(velocity);
    friction = Vector2Scale(norm_velocity, friction_c * -1);

    if (Vector2Length(friction) > Vector2Length(velocity))
        velocity = friction = Vector2Zero();

    velocity = Vector2Add(velocity, friction);
}


void Ball::update()
{
    apply_acceleration();
    apply_friction();
    set_x(position.x + velocity.x);
    set_y(position.y + velocity.y);
}


void Ball::roll(const Vector2 & kick_direction, float power)
{
    Vector2 kick_velocity = Vector2Scale(kick_direction, power);
    acceleration = Vector2Subtract(kick_velocity, velocity);
}


void Ball::kick(const Vector2 &kick_direction, float power)
{
    roll(kick_direction, power);
    controlled_by = nullptr;
    PlaySound(kick_sound);
}


void Ball::check_collision(Player & p, Bot & bot)
{
    if (controlled_by != nullptr)
        return;

    if (CheckCollisionCircleRec(position, radius, p.rec))
    {
        if (abs(velocity.x) > p.max_speed or abs(velocity.y) > p.max_speed)
            velocity = Vector2Negate(velocity);
        else
        {
            controlled_by = &p;
            velocity = Vector2Zero();
        }
    }
    else if (controlled_by != &bot && CheckCollisionCircleRec(position, radius, bot.rec))
       controlled_by = &bot;
}
