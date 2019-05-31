#include <iostream>
#include "bot.h"
#include "ball.h"

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;


Bot::Bot(float in_x, float in_y) : Player(in_x, in_y)
{
    max_speed = 3.f;
};


void Bot::update(Ball &ball, Player &p)
{
    //if (has_ball)
    //{
    //    Vector2 power;
    //    power.x = -GetRandomValue(50, 100);
    //    power.y = GetRandomValue(50, 100);
    //    ball.kick(power);
    //    return;
    //}

    //if (ball.direction.x <= 0)
    //{
    //    if (position.x > SCREENWIDTH/2  && ((ball.position.x + ball.radius) < position.x))
    //        set_x(position.x - movement_speed);
    //}
    //else
    //    // fall back
    //    set_x(position.x + movement_speed);

    //// move relative to ball
    //if ((ball.radius + ball.position.y) < (position.y + 20))
    //    set_y(position.y - movement_speed);
    //else if ((ball.radius + ball.position.y) > (position.y + 20))
    //    set_y(position.y + movement_speed);
}