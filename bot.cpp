#include <iostream>

#include "bot.h"
#include "ball.h"

extern const int LEFT_BOUND;
extern const int RIGHT_BOUND;
extern const int TOP_BOUND;
extern const int BOTTOM_BOUND;


Bot::Bot() : Player()
{
    max_speed = 3.f;
};


void Bot::setup()
{
    position.x = GetRandomValue(((RIGHT_BOUND - LEFT_BOUND) / 2) + LEFT_BOUND, RIGHT_BOUND - size.x);
    position.y = GetRandomValue(TOP_BOUND, BOTTOM_BOUND - size.y);
    velocity.x = 0;
    velocity.y = 0;
    power = 0;
}


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
    //    if (position.x > ((RIGHT_BOUND - LEFT_BOUND)/2 + LEFT_BOUND)  && ((ball.position.x + ball.radius) < position.x))
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