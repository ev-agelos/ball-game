#include <cmath>
#include <raymath.h>

#include "player.hpp"
#include "ball.hpp"
#include "utils.hpp"

extern bool COLLISION;
extern Rectangle FIELD;
extern Rectangle RIGHT_NET;
static const int APPROACH_RADIUS = 30;


Player::Player(const float max_speed)
    :
    acceleration_factor(300.f),
    deceleration_factor(0.1f),
    controls_ball(false),
    acceleration{0, 0},
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


void Player::add_acceleration(float dt, const Vector2& ball_pos)
{
    if (not input.x and not input.y)
    {
        acceleration = {0, 0};
        return;
    }

    Vector2 direction;
    if (controls_ball)
    {
        Vector2 rec_center = {rec.x + rec.width / 2, rec.y + rec.height / 2};
        Vector2 diff = Vector2Subtract(ball_pos, rec_center);
        direction = Vector2Normalize(diff);
    }
    else
        direction = input;

    acceleration = Vector2Scale(direction, acceleration_factor);

    limit_vector(acceleration, acceleration_factor);
    acceleration = Vector2Scale(acceleration, dt);
    velocity = Vector2Add(velocity, acceleration);
}


void Player::add_friction(float dt, const Vector2& ball_pos, float ball_radius)
{
    if (input.x or input.y or (not velocity.x and not velocity.y))
        return;

    if (controls_ball)
    {
        Vector2 nearest = get_nearest_rec_point(ball_pos, rec);
        float distance = Vector2Distance(ball_pos, nearest) - ball_radius - 1;  // -1 so they don't collide
        if (distance < APPROACH_RADIUS)
        {
            Vector2 desired_dir = Vector2Subtract(ball_pos, nearest);
            velocity = Vector2Scale(Vector2Normalize(desired_dir), distance * max_speed / APPROACH_RADIUS);
            // FIXME need to subtract here velocity- desired
        }
    }
    else
        velocity = Vector2Scale(velocity, pow(deceleration_factor, dt));

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


void Player::update(float dt, Ball& ball)
{
    if (input.x or input.y)
        input = Vector2Normalize(input);
    add_acceleration(dt, ball.position);
    add_friction(dt, ball.position, ball.radius);
    velocity = limit_vector(velocity, max_speed);
    update_pos(Vector2Scale(velocity, dt));
}


const Vector2 Player::get_kick_direction(const Vector2& ball_pos) const
{
    if (input.x || input.y)
        return input;

    Vector2 rec_center = {rec.x + rec.width / 2, rec.y + rec.height / 2};
    return Vector2Normalize(Vector2Subtract(ball_pos, rec_center));
}


void Player::handle_collision_response(Ball& ball, const Vector2& velocity, float dt)
{
    COLLISION = true;
    if (Vector2DotProduct(input, Vector2Normalize(this->velocity)) < -0.99)
    {
        kick(ball);
        return;
    }

    rec.x = last_position.x;
    rec.y = last_position.y;
    this->velocity = velocity;
    update_pos(Vector2Scale(this->velocity, dt));

    // prevent getting control of the ball if it is moving too fast
    if (abs(ball.velocity.x) <= max_speed and abs(ball.velocity.y) <= max_speed)
    {
        kick(ball);
        controls_ball = true;
    }
}
