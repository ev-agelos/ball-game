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
const int APPROACH_RADIUS = 30;


Player::Player()
    :
    ball_collision(false),
    acceleration({0, 0}),
    acceleration_factor(0.1),
    deceleration_factor(0.05),
    max_speed(2.f),
    position({0, 0}),
    size({20, 20}),
    direction({0, 0}),
    velocity({0, 0}),
    power(0)
{
}


void Player::setup()
{
    position.x = GetRandomValue(LEFT_BOUND + size.x/2, ((RIGHT_BOUND - LEFT_BOUND - size.x/2) / 2) + LEFT_BOUND);
    position.y = GetRandomValue(TOP_BOUND + size.y/2, BOTTOM_BOUND - size.y/2);
    velocity.x = 0;
    velocity.y = 0;
    power = 0;
}


void Player::update_pos(const Vector2 &v)
{
    float new_x = position.x + v.x;
    if ((new_x - size.x/2) >= LEFT_BOUND && (new_x + size.x/2) <= RIGHT_BOUND)
        position.x = new_x;

    float new_y = position.y + v.y;
    if ((new_y - size.y/2) >= TOP_BOUND && (new_y + size.y/2) <= BOTTOM_BOUND)
        position.y = new_y;
}


void Player::read_user_input()
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


void Player::apply_acceleration()
{
    Vector2 norm_direction = normalize_vector(direction);
    acceleration.x = norm_direction.x * acceleration_factor;
    acceleration.y = norm_direction.y * acceleration_factor;
}


void Player::set_velocity()
{
    if (acceleration.x)
        velocity.x += acceleration.x;
    else if (velocity.x > 0)
    {
        velocity.x -= deceleration_factor;
        if (velocity.x < 0)
            velocity.x = 0;
    }
    else if (velocity.x < 0)
    {
        velocity.x += deceleration_factor;
        if (velocity.x > 0)
            velocity.x = 0;
    }

    if (acceleration.y)
        velocity.y += acceleration.y;
    else if (velocity.y > 0)
    {
        velocity.y -= deceleration_factor;
        if (velocity.y < 0)
            velocity.y = 0;
    }
    else if (velocity.y < 0)
    {
        velocity.y += deceleration_factor;
        if (velocity.y > 0)
            velocity.y = 0;
    }

    limit_vector(velocity, max_speed);
}


void Player::handle_movement_control(Ball & ball)
{
    apply_acceleration();
    // Reset flag when ball comes outside rectangle's body
    if (ball_collision and !CheckCollisionCircleRec(ball.position, ball.radius, {position.x - size.x/2, position.y - size.y/2, size.x, size.y}))
        ball_collision = false;

    if (ball.controlled_by != this or ball_collision)
    {
        set_velocity();
        update_pos(velocity);
        return;
    }

    // User controls the ball
    if (CheckCollisionCircleRec(ball.position, ball.radius, {position.x - size.x/2, position.y - size.y/2, size.x, size.y}))
    {
        if (!ball_collision) // avoid calling roll() while ball is inside rectangle's body
        {
            if (power && IsKeyUp(KEY_D))
            {
                ball.kick(get_kick_direction(ball.position), power);
                power = 0;
            }
            else
                ball.roll(get_kick_direction(ball.position), 3);
            ball_collision = true;
            return;
        }
    }
    else if (!direction.x and !direction.y)  // no input so slow down
    {
        float nearest_x = std::max(position.x - size.x / 2, std::min(ball.position.x, position.x + size.x / 2));
        float nearest_y = std::max(position.y - size.y/2, std::min(ball.position.y, position.y + size.y/2));
        float distance = get_magnitude({ball.position.x - nearest_x, ball.position.y - nearest_y}) - ball.radius - 1; // -1 so they don't collide
        if (distance < APPROACH_RADIUS)
        {
            float speed = distance / APPROACH_RADIUS * max_speed;
            Vector2 desired_dir = normalize_vector({ball.position.x - position.x, ball.position.y - position.y});
            Vector2 desired_velocity = {desired_dir.x * speed, desired_dir.y * speed};
            acceleration = {desired_velocity.x - velocity.x, desired_velocity.y - velocity.y};
        }
    }
    else
    {
        // seek ball
        float dx = ball.position.x - position.x;
        float dy = ball.position.y - position.y;
        Vector2 desired_dir = normalize_vector({dx, dy});
        Vector2 desired_velocity = {desired_dir.x * max_speed, desired_dir.y * max_speed};
        acceleration = {desired_velocity.x - velocity.x, desired_velocity.y - velocity.y};
        limit_vector(acceleration, acceleration_factor);
    }
    set_velocity();
    update_pos(velocity);
}


void Player::update(Ball & ball)
{
    read_user_input();
    handle_movement_control(ball);

    if (IsKeyDown(KEY_D) && power < 100)
        power += 1;
}


const Vector2 Player::get_kick_direction(const Vector2 &ball_pos) const
{
    if (direction.x || direction.y)
        return normalize_vector(direction);
    else
    {
        float dx = ball_pos.x - position.x;
        float dy = ball_pos.y - position.y;
        return normalize_vector({dx, dy});
    }
}