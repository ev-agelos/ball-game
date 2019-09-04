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
    if (((val - radius) <= 0) || ((val + radius) >= SCREENWIDTH))
        velocity.x *= -1;

    if ((val - radius) >= 0 && (val + radius) <= SCREENWIDTH)
        position.x = val;
}


void Ball::set_y(float val)
{
    if (((val - radius) <= 0) || ((val + radius) >= SCREENHEIGHT))
        velocity.y *= -1;

    if ((val - radius) >= 0 && (val + radius) <= SCREENHEIGHT)
        position.y = val;
}


void Ball::update(Player & p, Bot & bot)
{
    if (velocity.x == 0 && velocity.y == 0)
        return;

    //if (velocity.x)
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
    // set the ball's position relative to player before kicking it
    float player_radius = sqrt(p.rec.width*p.rec.width + p.rec.height*p.rec.height) / 2;
    float player_center_x = p.rec.x + p.rec.width/2;
    float player_center_y = p.rec.y + p.rec.height/2;
    direction = normalize_vector(p.direction);
    float new_x, new_y;
    if (p.direction.x == 0 && p.direction.y == 0)
    {
        float dx = position.x - p.rec.x;
        float dy = position.y - p.rec.y;
        float magnitude = std::sqrt(dx*dx + dy*dy);
        new_x = dx / magnitude;
        new_y = dy / magnitude;
    }
    else
    {
        new_x = direction.x;
        new_y = direction.y;
        set_x(player_center_x + new_x*(player_radius + radius));
        set_y(player_center_y + new_y*(player_radius + radius));
    }
    velocity.x = new_x * power;
    velocity.y = new_y * power;

    PlaySound(kick_sound);
}


void Ball::check_collision(Player & p, Bot & bot)
{
    //if (CheckCollisionCircleRec(position, radius, {p.position.x, p.position.y, p.size.x, p.size.y}))
    //{
    //    direction.x = 0;
    //    direction.y = 0;
    //}
    //else if (!bot.has_ball && CheckCollisionCircleRec(position, radius, {bot.position.x, bot.position.y, bot.size.x, bot.size.y}))
    //{
    //    direction.x = 0;
    //    direction.y = 0;
    //    p.has_ball = false;
    //    bot.has_ball = true;
    //}

    // TODO check collisions with walls and set the ball to stop or outside or bounce
}