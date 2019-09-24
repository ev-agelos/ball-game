#include <iostream>
#include <math.h>
#include <raylib.h>
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

Ball::Ball(Sound sound)
    :
    acceleration_factor(4),
    friction_c(0.02),
    radius(5.f),
    acceleration({0, 0}),
    friction({0, 0}),
    velocity({0, 0}),
    controlled_by(nullptr),
    crossed_net(false),
    left_score(0),
    right_score(0),
    kick_sound(sound)
{
}


void Ball::setup()
{
    position.x = ((RIGHT_BOUND - LEFT_BOUND) / 2.f) + LEFT_BOUND;
    position.y = ((BOTTOM_BOUND - TOP_BOUND) / 2.f) + TOP_BOUND;
    velocity.x = 0;
    velocity.y = 0;
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
                right_score += 1;
                crossed_net = true;
            }
            else if (position.x > RIGHT_BOUND)
            {
                left_score += 1;
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
    if (abs(acceleration.x) > acceleration_factor)
    {
        if (acceleration.x > 0)
        {
            velocity.x += acceleration_factor;
            acceleration.x -= acceleration_factor;
        }
        else if (acceleration.x < 0)
        {
            velocity.x -= acceleration_factor;
            acceleration.x += acceleration_factor;
        }
    }
    else
    {
        velocity.x += acceleration.x;
        acceleration.x = 0;
    }

    if (abs(acceleration.y) > acceleration_factor)
    {
        if (acceleration.y > 0)
        {
            velocity.y += acceleration_factor;
            acceleration.y -= acceleration_factor;
        }
        else if (acceleration.y < 0)
        {
            velocity.y -= acceleration_factor;
            acceleration.y += acceleration_factor;
        }
    }
    else
    {
        velocity.y += acceleration.y;
        acceleration.y = 0;
    }
}


void Ball::apply_friction()
{
    Vector2 norm_velocity = normalize_vector(velocity);
    friction.x = norm_velocity.x * friction_c * -1;
    friction.y = norm_velocity.y * friction_c * -1;
    
    if (abs(friction.x) >= abs(velocity.x))
        velocity.x = friction.x = 0;
    if (abs(friction.y) >= abs(velocity.y))
        velocity.y = friction.y = 0;

    velocity.x += friction.x;
    velocity.y += friction.y;
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
    acceleration.x = kick_direction.x * power - velocity.x;
    acceleration.y = kick_direction.y * power - velocity.y;
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

    if (CheckCollisionCircleRec(position, radius, {p.position.x - p.size.x/2, p.position.y - p.size.y/2, p.size.x, p.size.y}))
    {
        
        if (velocity.x > p.max_speed)
            velocity.x *= -1;
        else if (velocity.y > p.max_speed)
            velocity.y *= -1;
        else
        {
            controlled_by = &p;
            velocity.x = 0;
            velocity.y = 0;
        }
    }
    else if (controlled_by != &bot && CheckCollisionCircleRec(position, radius, {bot.position.x - bot.size.x/2, bot.position.y - bot.size.y/2, bot.size.x, bot.size.y}))
       controlled_by = &bot;
}
