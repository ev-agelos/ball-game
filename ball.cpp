#include <iostream>
#include <math.h>
#include <raylib.h>
#include "ball.h"
#include "player.h"
#include "bot.h"
#include "utils.h"

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;


Ball::Ball()
    :
    deceleration_factor(0.97),
    radius(5.f),
    position({ SCREENWIDTH / 2.f, SCREENHEIGHT / 2.f }),
    velocity({ 0, 0 })
{
}


void Ball::set_x(float val)
{
    // check if hit vertical wall
    if (((val - radius) <= 0) || ((val + radius) >= SCREENWIDTH))
    {
        velocity.x *= -1;
        controlled_by = nullptr;
    }

    if ((val - radius) >= 0 && (val + radius) <= SCREENWIDTH)
        position.x = val;
}


void Ball::set_y(float val)
{
    // check if hit horizontal wall
    if (((val - radius) <= 0) || ((val + radius) >= SCREENHEIGHT))
    {
        velocity.y *= -1;
        controlled_by = nullptr;
    }

    if ((val - radius) >= 0 && (val + radius) <= SCREENHEIGHT)
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
