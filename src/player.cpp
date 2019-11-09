#include <cmath>
#include <raylib.h>
#include <raymath.h>

#include "player.hpp"
#include "ball.hpp"
#include "utils.hpp"

extern Rectangle FIELD;
extern Rectangle RIGHT_NET;
static const int APPROACH_RADIUS = 30;


Player::Player(const float max_speed)
    :
    acceleration{0, 0},
    acceleration_factor(300.f),
    deceleration_factor(0.1f),
    controls_ball(false),
    input{0, 0},
    max_speed(max_speed),
    rec{0, 0, 20, 20},
    velocity{0, 0},
    power(0),
    last_position{0, 0}
{
    setup();
}


void Player::setup()
{
    rec.x = GetRandomValue(FIELD.x, FIELD.width / 2 + FIELD.x - rec.width);
    rec.y = GetRandomValue(FIELD.y, FIELD.y + FIELD.height - rec.height);
    velocity = {0, 0};
    power = 0;
    controls_ball = false;
}


void Player::update_pos(const Vector2& v)
{
    last_position = {rec.x, rec.y};

    float new_x = rec.x + v.x;
    if (new_x >= FIELD.x and (new_x + rec.width) <= RIGHT_NET.x)
        rec.x = new_x;

    float new_y = rec.y + v.y;
    if (new_y >= FIELD.y and (new_y + rec.height) <= FIELD.y + FIELD.height)
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

    if (input.x or input.y)
        input = Vector2Normalize(input);
}


void Player::apply_acceleration(float& dt)
{
    if (input.x or input.y)
        acceleration = Vector2Scale(input, acceleration_factor * dt);
    else
        acceleration = {0, 0};
}


void Player::set_velocity()
{
    if (acceleration.x or acceleration.y)
    {
        velocity = Vector2Add(velocity, acceleration);
        velocity = limit_vector(velocity, max_speed);
    }
}


void Player::kick(Ball& ball)
{
    Vector2 kick_direction = get_kick_direction(ball.position);
    if (power && IsKeyUp(KEY_D))
    {
        ball.kick(kick_direction, power);
        power = 0;
        controls_ball = false;
    }
    else
        ball.roll(kick_direction, 150);
}


void Player::handle_movement_control(float& dt, Ball& ball)
{
    Vector2 rec_center = {rec.x + rec.width / 2, rec.y + rec.height / 2};
    Vector2 diff = Vector2Subtract(ball.position, rec_center);
    Vector2 desired_dir = Vector2Normalize(diff);
    Vector2 desired_velocity = Vector2Scale(desired_dir, max_speed);
    acceleration = Vector2Subtract(desired_velocity, velocity);
    acceleration = Vector2Scale(acceleration, dt);
}


void Player::update(float& dt, Ball& ball)
{
    read_user_input();

    if (controls_ball and (input.x or input.y))
    {
        handle_movement_control(dt, ball);
        limit_vector(acceleration, acceleration_factor);
    }
    else
        apply_acceleration(dt);

    // change in velocity
    if (acceleration.x or acceleration.y)
        set_velocity();
    else if (velocity.x or velocity.y)
    {
        Vector2 nearest = get_nearest_rec_point(ball.position, rec);
        float distance = Vector2Distance(ball.position, nearest) - ball.radius - 1;  // -1 so they don't collide
        if (distance < APPROACH_RADIUS)
        {
            Vector2 desired_dir = Vector2Subtract(ball.position, nearest);
            velocity = Vector2Scale(Vector2Normalize(desired_dir), distance * max_speed / APPROACH_RADIUS);
        }
        else
            velocity = Vector2Scale(velocity, pow(deceleration_factor, dt));
    }
    update_pos(Vector2Scale(velocity, dt));
}


const Vector2 Player::get_kick_direction(const Vector2& ball_pos) const
{
    if (input.x || input.y)
        return input;

    Vector2 rec_center = {rec.x + rec.width / 2, rec.y + rec.height / 2};
    return Vector2Normalize(Vector2Subtract(ball_pos, rec_center));
}


void Player::handle_collision_response(Ball& ball, const Vector2& velocity, float& dt)
{
    if (Vector2DotProduct(input, Vector2Normalize(this->velocity)) < -0.99)
    {
        acceleration = Vector2Negate(acceleration);
        set_velocity();
        return;
    }

    // ball will collide with player after the kick, so let player move freely
    if (Vector2DotProduct(Vector2Normalize(this->velocity), input) < 0)
    {
        set_velocity();
        kick(ball);
        controls_ball = true;
        return;
    }
    rec.x = last_position.x;
    rec.y = last_position.y;
    this->velocity = velocity;
    update_pos(Vector2Scale(this->velocity, dt));

    // prevent getting control of the ball if it is moving too fast
    if ((abs(ball.velocity.x) <= max_speed and abs(this->velocity.y) <= max_speed) and (input.x or input.y))
    {
        kick(ball);
        controls_ball = true;
    }
}
