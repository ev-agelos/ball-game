#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
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
    ball_inside_rectangle(false),
    acceleration{0, 0},
    acceleration_factor(0.1),
    deceleration_factor(0.05),
    max_speed(2.f),
    position{0, 0},
    size{20, 20},
    direction{0, 0},
    velocity{0, 0},
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
    if (!direction.x and !direction.y)
        return;

    Vector2 norm_direction = Vector2Normalize(direction);
    acceleration = Vector2Scale(norm_direction, acceleration_factor);
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


void Player::kick(Ball &ball)
{
    Vector2 kick_direction = get_kick_direction(ball.position);
    if (power && IsKeyUp(KEY_D))
    {
        ball.kick(kick_direction, power);
        power = 0;
    }
    else
        ball.roll(kick_direction, 1);

    return;
}


void Player::handle_movement_control(Ball & ball)
{
    Rectangle rec = {position.x - size.x / 2, position.y - size.y / 2, size.x, size.y};
    if (!CheckCollisionCircleRec(ball.position, ball.radius, rec))
        ball_inside_rectangle = false;

    apply_acceleration();

    if (ball.controlled_by != this)
    {
        set_velocity();
        if (!velocity.x and !velocity.y)
            return;

        Vector2 last_position = position;
        Vector2 last_velocity = velocity;
        velocity = get_constrained_velocity(ball, velocity);
        position = last_position;
        if (velocity.x != last_velocity.x or velocity.y != last_velocity.y)
        {
            kick(ball);
            ball.controlled_by = this;
        }
    }
    else if (!direction.x and !direction.y)
    {
        float nearest_x = Clamp(ball.position.x, position.x - size.x / 2, position.x + size.x / 2);
        float nearest_y = Clamp(ball.position.y, position.y - size.y / 2, position.y + size.y / 2);
        float distance = Vector2Length({ball.position.x - nearest_x, ball.position.y - nearest_y}) - ball.radius - 1;  // -1 so they don't collide
        if (distance < APPROACH_RADIUS)
        {
            float speed = distance / APPROACH_RADIUS * max_speed;
            Vector2 desired_dir = Vector2Normalize(Vector2Subtract(ball.position, position));
            Vector2 desired_velocity = Vector2Scale(desired_dir, speed);
            acceleration = Vector2Subtract(desired_velocity, velocity);
        }
        set_velocity();
    }
    else if (ball_inside_rectangle)
    {
        acceleration = Vector2Negate(acceleration);
        set_velocity();
    }
    else
    {
        Vector2 diff = Vector2Subtract(ball.position, position);
        Vector2 desired_dir = Vector2Normalize(diff);
        Vector2 desired_velocity = Vector2Scale(desired_dir, max_speed);
        acceleration = Vector2Subtract(desired_velocity, velocity);
        limit_vector(acceleration, acceleration_factor);
        set_velocity();

        Vector2 last_position = position;
        Vector2 last_velocity = velocity;
        velocity = get_constrained_velocity(ball, velocity);
        position = last_position;
        if (velocity.x != last_velocity.x or velocity.y != last_velocity.y)
        {
            kick(ball);
            if (velocity.x * ball.acceleration.x < 0 or velocity.y * ball.acceleration.y < 0)
            {
                velocity = last_velocity;
                ball_inside_rectangle = true;
            }
        }
    }
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
        return Vector2Normalize(direction);
    else
        return Vector2Normalize(Vector2Subtract(ball_pos, position));
}


float get_penetration_distance(Vector2 rec_pos, Vector2 rec_size, Vector2 ball_pos, float ball_radius)
{
    float nearest_x = Clamp(ball_pos.x, rec_pos.x - rec_size.x / 2, rec_pos.x + rec_size.x / 2);
    float nearest_y = Clamp(ball_pos.y, rec_pos.y - rec_size.y / 2, rec_pos.y + rec_size.y / 2);
    float distance = Vector2Length({ball_pos.x - nearest_x, ball_pos.y - nearest_y});
    return ball_radius - distance;
}


Vector2 Player::get_constrained_velocity(Ball &ball, Vector2 new_velocity)
{
    float distance = Vector2Length(new_velocity);
    Vector2 norm_new_velocity = Vector2Normalize(new_velocity);
    for (int i = 0; i <= distance; i++)
    {
        Vector2 next_pos = {position.x + (i * norm_new_velocity.x), position.y + (i * norm_new_velocity.y)};
        Rectangle rec = {next_pos.x - size.x / 2, next_pos.y - size.y / 2, size.x, size.y};
        if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
        {
            float penetration_distance = get_penetration_distance(next_pos, size, ball.position, ball.radius);
            return Vector2Scale(norm_new_velocity, (i - penetration_distance));
        }
    }

    // check distance in case it is a float ex: 5.2
    Vector2 next_pos = {position.x + (distance * norm_new_velocity.x), position.y + (distance * norm_new_velocity.y)};
    Rectangle rec = {next_pos.x - size.x / 2, next_pos.y - size.y / 2, size.x, size.y};
    if (CheckCollisionCircleRec(ball.position, ball.radius, rec))
    {
        float penetration_distance = get_penetration_distance(next_pos, size, ball.position, ball.radius);
        return Vector2Scale(norm_new_velocity, (distance - penetration_distance));
    }

    return new_velocity;  // no collision
}
