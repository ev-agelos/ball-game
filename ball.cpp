#include <iostream>
#include <math.h>
#include <raylib.h>
#include "ball.h"
#include "player.h"
#include "bot.h"
#include "utils.h"

extern const int UPPER_BOUND;
extern const int LOWER_BOUND;
extern const int LEFT_BOUND;
extern const int RIGHT_BOUND;
extern const int GOALPOST_THICKNESS;
extern const float GOALPOST_HEIGHT_START;
extern const float GOALPOST_HEIGHT_END;


Ball::Ball()
    :
    deceleration_factor(0.97),
    radius(5.f),
    position({ ((RIGHT_BOUND - LEFT_BOUND) / 2.f) + LEFT_BOUND, ((LOWER_BOUND - UPPER_BOUND) / 2.f) + UPPER_BOUND }),
    velocity({ 0, 0 })
{
}


void Ball::set_x(float val)
{
    // hit vertical wall or goal
    if (((val - radius) <= LEFT_BOUND) || ((val + radius) >= RIGHT_BOUND))
    {
        if (position.y < (GOALPOST_HEIGHT_START + GOALPOST_THICKNESS) || position.y > GOALPOST_HEIGHT_END)
            velocity.x *= -1;
        
        controlled_by = nullptr;
    }

    position.x = val;
}


void Ball::set_y(float val)
{
    // hit horizontal wall or goal
    if (((val - radius) <= UPPER_BOUND) || ((val + radius) >= LOWER_BOUND))
    {
        velocity.y *= -1;
        controlled_by = nullptr;
    }

    position.y = val;
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
    // set ball's direction the one that player points to
    direction.x = p.direction.x;
    direction.y = p.direction.y;
    Vector2 norm_direction = normalize_vector(direction);
    float player_center_x = p.rec.x + p.rec.width/2;
    float player_center_y = p.rec.y + p.rec.height/2;

    // set the ball's position relative to player update and then "kick" it
    float player_radius = std::sqrt(p.rec.width*p.rec.width + p.rec.height*p.rec.height) / 2;
    velocity.x = (player_center_x - position.x) + norm_direction.x * (player_radius + radius);
    velocity.y = (player_center_y - position.y) + norm_direction.y * (player_radius + radius);
    update();

    velocity.x = norm_direction.x*power;
    velocity.y = norm_direction.y*power;

    PlaySound(kick_sound);
}


void Ball::check_collision(Player & p, Bot & bot)
{
    if (CheckCollisionCircleRec(position, radius, {p.rec.x, p.rec.y, p.rec.width, p.rec.height}))
        controlled_by = &p;
    else if (controlled_by != &bot && CheckCollisionCircleRec(position, radius, {bot.rec.x, bot.rec.y, bot.rec.width, bot.rec.height}))
       controlled_by = &bot;
}
