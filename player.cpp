#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <raylib.h>
#include "player.h"
#include "ball.h"
#include "utils.h"

extern const int TOP_BOUND;
extern const int BOTTOM_BOUND;
extern const int LEFT_BOUND;
extern const int RIGHT_BOUND;


Player::Player()
    :
    max_speed(2.f),
    acceleration({0, 0}),
    acceleration_factor(0.1),
    deceleration_factor(0.05),
    rec({0, 0, 20.f, 20.f}),
    direction({0, 0}),
    velocity({0, 0}),
    power(0)
{
}


void Player::setup()
{
    rec.x = GetRandomValue(LEFT_BOUND, ((RIGHT_BOUND - LEFT_BOUND - rec.width) / 2) + LEFT_BOUND);
    rec.y = GetRandomValue(TOP_BOUND, BOTTOM_BOUND - rec.height);
    velocity.x = 0;
    velocity.y = 0;
}


void Player::update_pos(const Vector2 &v)
{
    float new_x = rec.x + v.x;
    if (new_x >= LEFT_BOUND && (new_x + rec.width) <= RIGHT_BOUND)
        rec.x = new_x;

    float new_y = rec.y + v.y;
    if (new_y >= TOP_BOUND && (new_y + rec.height) <= BOTTOM_BOUND)
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


void Player::set_acceleration()
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

    Vector2 norm_dir = normalize_vector(direction);
    acceleration.x = norm_dir.x * acceleration_factor;
    acceleration.y = norm_dir.y * acceleration_factor;
}


void Player::set_velocity()
{
    if (not direction.x and velocity.x < 0)
        velocity.x += deceleration_factor;
    else if (not direction.x and velocity.x > 0)
        velocity.x -= deceleration_factor;
    else
        velocity.x += acceleration.x;

    if (not direction.y and velocity.y < 0)
        velocity.y += deceleration_factor;
    else if (not direction.y and velocity.y > 0)
        velocity.y -= deceleration_factor;
    else
        velocity.y += acceleration.y;

    set_valid_velocity(velocity.x);
    set_valid_velocity(velocity.y);
}


void Player::handle_movement_control(Ball & ball)
{
    set_acceleration();
    if (ball.controlled_by != this)
    {
        set_velocity();
        update_pos(velocity);
        return;
    }

    // User controls the ball
    if (direction.x == 0 and direction.y == 0)  // no input so keep ball's direction
    {
        set_velocity();
        update_pos(velocity);
    }
    else if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
        ball.roll(*this, 3);
    else if (dot_product(normalize_vector(velocity), normalize_vector(ball.velocity)) == -1)
    {
        set_velocity();
        update_pos(velocity);
    }
    else
    {
        // keep player following ball cause he controls it
        float player_center_x = rec.x + rec.width/2;
        float player_center_y = rec.y + rec.height/2;
        float dx = ball.position.x - player_center_x;
        float dy = ball.position.y - player_center_y;
        Vector2 desired_dir = normalize_vector({dx, dy});
        velocity = {desired_dir.x * max_speed, desired_dir.y * max_speed};
        update_pos(velocity);
    }
}


void Player::update(Ball & ball)
{
    handle_movement_control(ball);

    if (IsKeyDown(KEY_S) && power < 100)
        power += 1;
    
    if (IsKeyReleased(KEY_S))
    {
        if (ball.controlled_by == this)
        {
            ball.kick(*this, power);
            ball.controlled_by = nullptr;
        }
        power = 0;
    }

}