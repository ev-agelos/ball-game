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
    ball_collision(false),
    acceleration({0, 0}),
    acceleration_factor(0.1),
    deceleration_factor(0.05),
    max_speed(2.f),
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

    acceleration.x = direction.x ? direction.x * acceleration_factor : 0;
    acceleration.y = direction.y ? direction.y * acceleration_factor : 0;
}


void Player::set_velocity()
{
    if (not direction.x and velocity.x)
    {
        velocity.x += (-velocity.x / abs(velocity.x)) * deceleration_factor;
        // zero out so player doesnt move on values close to zero
        if ((velocity.x > 0 && velocity.x < 0.1) || (velocity.x < 0 && velocity.x > -0.1))
            velocity.x = 0;
    }
    else
        velocity.x += acceleration.x;

    if (not direction.y and velocity.y)
    {
        velocity.y += (-velocity.y / abs(velocity.y)) * deceleration_factor;
        // zero out so player doesnt move on values close to zero
        if ((velocity.y > 0 && velocity.y < 0.1) || (velocity.y < 0 && velocity.y > -0.1))
            velocity.y = 0;
    }
    else
        velocity.y += acceleration.y;

    set_valid_velocity(velocity.x);
    set_valid_velocity(velocity.y);
}


void Player::handle_movement_control(Ball & ball)
{
    set_acceleration();
    // Reset flag when ball comes outside rectangle's body
    if (!CheckCollisionCircleRec(ball.position, ball.radius, rec))
        ball_collision = false;

    if (ball.controlled_by != this)
    {
        set_velocity();
        update_pos(velocity);
        return;
    }

    // User controls the ball
    if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
    {
        if (!ball_collision) // avoid calling roll() while ball is inside rectangle's body
        {
            if (power && IsKeyUp(KEY_S))
            {
                ball.kick(get_kick_direction(ball.position), power);
                power = 0;
            }
            else
                ball.roll(get_kick_direction(ball.position), 3);
            ball_collision = true;
        }
        else
        {
            set_velocity();
            update_pos(velocity);
        }
        
    }
    else if (direction.x == 0 and direction.y == 0)  // no input so keep ball's direction
    {
        set_velocity();
        update_pos(velocity);
    }
    else if (dot_product(normalize_vector(velocity), normalize_vector(ball.velocity)) == -1)
    {
        set_velocity();
        update_pos(velocity);
    }
    else
    {
        // seek ball
        float player_center_x = rec.x + rec.width/2;
        float player_center_y = rec.y + rec.height/2;
        float dx = ball.position.x - player_center_x;
        float dy = ball.position.y - player_center_y;
        Vector2 desired_dir = normalize_vector({dx, dy});
        Vector2 desired_velocity = {desired_dir.x * max_speed, desired_dir.y * max_speed};
        acceleration = {desired_velocity.x - velocity.x, desired_velocity.y - velocity.y};

        if (abs(acceleration.x) > acceleration_factor)
            acceleration.x = (acceleration.x / abs(acceleration.x)) * acceleration_factor;
        if (abs(acceleration.y) > acceleration_factor)
            acceleration.y = (acceleration.y / abs(acceleration.y)) * acceleration_factor;

        // Bypass calling set_velocity as it will slow down cause depends on user input
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        update_pos(velocity);
    }
}


void Player::update(Ball & ball)
{
    handle_movement_control(ball);

    if (IsKeyDown(KEY_S) && power < 100)
        power += 1;
}


const Vector2 Player::get_kick_direction(const Vector2 &ball_pos) const
{
    if (direction.x || direction.y)
        return normalize_vector(direction);
    else
    {
        float center_x = rec.x + rec.width / 2;
        float center_y = rec.y + rec.height/2;
        float dx = ball_pos.x - center_x;
        float dy = ball_pos.y - center_y;
        return normalize_vector({dx, dy});
    }
}