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
    rec{0, 0, 20, 20},
    input{0, 0},
    velocity{0, 0},
    power(0),
    last_position{0, 0}
{
}


void Player::setup()
{
    rec.x = GetRandomValue(LEFT_BOUND, ((RIGHT_BOUND - LEFT_BOUND - rec.width) / 2) + LEFT_BOUND);
    rec.y = GetRandomValue(TOP_BOUND, BOTTOM_BOUND - rec.height);
    velocity = {0, 0};
    power = 0;
}


void Player::update_pos(const Vector2 &v)
{
    last_position = {rec.x, rec.y};

    float new_x = rec.x + v.x;
    if (new_x >= LEFT_BOUND && (new_x + rec.width) <= RIGHT_BOUND)
        rec.x = new_x;

    float new_y = rec.y + v.y;
    if (new_y >= TOP_BOUND && (new_y + rec.height) <= BOTTOM_BOUND)
        rec.y = new_y;
}


void Player::read_user_input()
{
    if (IsKeyDown(KEY_RIGHT) and IsKeyDown(KEY_LEFT))
        input.x = 0;
    else if (IsKeyDown(KEY_RIGHT))
        input.x = 1;
    else if (IsKeyDown(KEY_LEFT))
        input.x = -1;
    else
        input.x = 0;

    if (IsKeyDown(KEY_UP) and IsKeyDown(KEY_DOWN))
        input.y = 0;
    else if (IsKeyDown(KEY_UP))
        input.y = -1;
    else if (IsKeyDown(KEY_DOWN))
        input.y = 1;
    else
        input.y = 0;
}


void Player::apply_acceleration()
{
    if (input.x or input.y)
    {
        Vector2 norm_input = Vector2Normalize(input);
        acceleration = Vector2Scale(norm_input, acceleration_factor);
    }
    else
        acceleration = {0, 0};
}


void Player::set_velocity()
{
    velocity = Vector2Add(velocity, acceleration);
    if (!velocity.x and !velocity.y)
        return;

    limit_vector(velocity, max_speed);

    if (!acceleration.x and !acceleration.y)
    {
        float length = Vector2Length(velocity);
        velocity = Vector2Scale(Vector2Normalize(velocity), length - deceleration_factor);
    }
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
}


void Player::handle_movement_control(Ball & ball)
{
    if (!input.x and !input.y)
    {
        Vector2 nearest = get_nearest_rec_point(ball.position, rec);
        float distance = Vector2Distance(ball.position, nearest) - ball.radius - 1;  // -1 so they don't collide
        if (distance < APPROACH_RADIUS)
        {
            float speed = distance / APPROACH_RADIUS * max_speed;
            Vector2 rec_center = {rec.x + rec.width / 2, rec.y + rec.height / 2};
            Vector2 desired_dir = Vector2Normalize(Vector2Subtract(ball.position, rec_center));
            Vector2 desired_velocity = Vector2Scale(desired_dir, speed);
            acceleration = Vector2Subtract(desired_velocity, velocity);
        }
    }
    else
    {
        Vector2 rec_center = {rec.x + rec.width / 2, rec.y + rec.height / 2};
        Vector2 diff = Vector2Subtract(ball.position, rec_center);
        Vector2 desired_dir = Vector2Normalize(diff);
        Vector2 desired_velocity = Vector2Scale(desired_dir, max_speed);
        acceleration = Vector2Subtract(desired_velocity, velocity);
        limit_vector(acceleration, acceleration_factor);
    }
}


void Player::update(Ball & ball)
{
    read_user_input();
    if (!CheckCollisionCircleRec(ball.position, ball.radius, rec))
        ball_inside_rectangle = false;
    apply_acceleration();
    if (ball.controlled_by == this)
        handle_movement_control(ball);
    set_velocity();
    update_pos(velocity);
    if (IsKeyDown(KEY_D) && power < 100)
        power += 1;
}


const Vector2 Player::get_kick_direction(const Vector2 &ball_pos) const
{
    if (input.x || input.y)
        return Vector2Normalize(input);
    else
    {
        Vector2 rec_center = {rec.x + rec.width / 2, rec.y + rec.height / 2};
        return Vector2Normalize(Vector2Subtract(ball_pos, rec_center));
    }
}


void Player::handle_collision_response(Ball &ball, Vector2 velocity)
{
    if (Vector2DotProduct(Vector2Normalize(input), Vector2Normalize(this->velocity)) < -0.99)
        ball_inside_rectangle = true;

    if (ball_inside_rectangle)
    {
        acceleration = Vector2Negate(acceleration);
        set_velocity();
        return;
    }
    rec.x = last_position.x;
    rec.y = last_position.y;
    this->velocity = velocity;
    update_pos(this->velocity);

    // prevent getting control of the ball if it is moving too fast
    if ((abs(ball.velocity.x) <= max_speed and abs(this->velocity.y) <= max_speed) and (input.x or input.y))
    {
        kick(ball);
        ball.controlled_by = this;
    }
}
