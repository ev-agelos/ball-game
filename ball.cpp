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
    deceleration_factor(0.97),
    radius(5.f),
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


void Ball::update()
{
    if (velocity.x == 0 && velocity.y == 0)
        return;

    set_x(position.x + velocity.x);
    velocity.x *= deceleration_factor;

    if (abs(velocity.x) < 0.1)
        velocity.x = 0;

    if (velocity.y)
    {
        set_y(position.y + velocity.y);
        velocity.y *= deceleration_factor;
        if (abs(velocity.y) < 0.1)
            velocity.y = 0;
    }
}


void Ball::roll(const Player & p, float power)
{
    Vector2 kick_direction;
    float player_center_x = p.rec.x + p.rec.width/2;
    float player_center_y = p.rec.y + p.rec.height/2;

    // set ball's direction the one that player points to
    if (p.direction.x || p.direction.y)
        kick_direction = normalize_vector(p.direction);
    else
    {
        float dx = position.x - player_center_x;
        float dy = position.y - player_center_y;
        kick_direction = normalize_vector({dx, dy});
    }

    velocity.x = kick_direction.x*power;
    velocity.y = kick_direction.y*power;
}


void Ball::kick(const Player &p, float power)
{
    roll(p, power);
    PlaySound(kick_sound);
}


void Ball::check_collision(Player & p, Bot & bot)
{
    if (controlled_by != nullptr)
        return;

    if (CheckCollisionCircleRec(position, radius, p.rec))
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
    else if (controlled_by != &bot && CheckCollisionCircleRec(position, radius, bot.rec))
       controlled_by = &bot;
}
