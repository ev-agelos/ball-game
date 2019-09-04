#include <iostream>
#include <algorithm>
#include <time.h>
#include <raylib.h>
#include "player.h"
#include "ball.h"
#include "utils.h"

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;


Player::Player(float x, float y)
    :
    max_speed(2.f),
    acceleration_factor(0.1),
    deceleration_factor(0.2),
    velocity({0, 0}),
    rec({x, y, 20.f, 20.f}),
    direction({0, 0}),
    power(0),
    has_ball(false)
{
}


void Player::update_pos(const Vector2 &v)
{
    float new_x = rec.x + v.x;
    if (new_x >= 0 && (new_x + rec.width) <= SCREENWIDTH)
        rec.x = new_x;

    float new_y = rec.y + v.y;
    if (new_y >= 0 && (new_y + rec.height) <= SCREENHEIGHT)
        rec.y = new_y;
}


void Player::set_valid_velocity(float &value)
{
    if (value > max_speed)
        value = max_speed;
    else if (value < -max_speed)
        value = -max_speed;
    else if ((value > 0 && value < 0.1) || (value < 0 && value > -0.1))
        value = 0;
}


void Player::set_direction()
{
    if (IsKeyDown(KEY_RIGHT) and IsKeyDown(KEY_LEFT))
        direction.x = 0;
    else if (IsKeyDown(KEY_RIGHT))
        direction.x = 1;
    else if (IsKeyDown(KEY_LEFT))
        direction.x = -1;
    else
        direction.x = 0;

    if (IsKeyDown(KEY_UP) and IsKeyDown(KEY_DOWN))
        direction.y = 0;
    else if (IsKeyDown(KEY_UP))
        direction.y = -1;
    else if (IsKeyDown(KEY_DOWN))
        direction.y = 1;
    else
        direction.y = 0;
}


void Player::set_velocity()
{
    if (not direction.x and velocity.x < 0)
        velocity.x += deceleration_factor;
    else if (not direction.x and velocity.x > 0)
        velocity.x -= deceleration_factor;
    else
        velocity.x += direction.x * acceleration_factor;

    if (not direction.y and velocity.y < 0)
        velocity.y += deceleration_factor;
    else if (not direction.y and velocity.y > 0)
        velocity.y -= deceleration_factor;
    else
        velocity.y += direction.y * acceleration_factor;

    set_valid_velocity(velocity.x);
    set_valid_velocity(velocity.y);
}


void Player::handle_movement_control(Ball & ball)
{
    set_direction();
    if (!has_ball)
    {
        // normal movement
        set_velocity();
        update_pos(velocity);
        if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
        {
            // "take" control of the ball
            ball.roll(*this, 3);
            has_ball = true;
        }
        return;
    }

    // User controls the ball
    if (direction.x == 0 and direction.y == 0)  // no input so roll with the ball
    {
        set_velocity();
        update_pos(ball.velocity);
    }
    else if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
    {
        // Avoid the slow down when changing to opposite direction
        if (dot_product(direction, ball.direction) == -1)
        {
            velocity.x *= -1;
            velocity.y *= -1;
        }
        ball.roll(*this, 3);
    }
    else
    {
        if (dot_product(direction, ball.direction) == -1)
        {
            direction.x = ball.direction.x;
            direction.y = ball.direction.y;
        }
        set_velocity();

        // Follow the ball
        float speed = std::max(abs(velocity.x), abs(velocity.y));
        update_pos({ball.direction.x*speed, ball.direction.y*speed});
    }
}


void Player::update(Ball & ball)
{
    handle_movement_control(ball);

    if (IsKeyDown(KEY_S) && power < 100)
        power += 1;
    
    if (IsKeyReleased(KEY_S))
    {
        if (has_ball)
        {
            ball.roll(*this, power);
            has_ball = false;
        }
        power = 0;
    }

}