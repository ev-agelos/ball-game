#include <iostream>
#include <algorithm>
#include <time.h>
#include <math.h>
#include <raylib.h>
#include "player.h"
#include "ball.h"
#include "utils.h"

extern const int UPPER_BOUND;
extern const int LOWER_BOUND;
extern const int LEFT_BOUND;
extern const int RIGHT_BOUND;


Player::Player()
    :
    max_speed(2.f),
    acceleration_factor(0.1),
    deceleration_factor(0.2),
    rec({0, 0, 20.f, 20.f}),
    direction({0, 0}),
    velocity({0, 0}),
    power(0)
{
}


void Player::setup()
{
    auto [pos_x, pos_y] = random_pos(LEFT_BOUND, ((RIGHT_BOUND - LEFT_BOUND) / 2) + LEFT_BOUND, LOWER_BOUND, UPPER_BOUND);
    rec.x = pos_x;
    rec.y = pos_y;
    velocity.x = 0;
    velocity.y = 0;
}


void Player::update_pos(const Vector2 &v)
{
    float new_x = rec.x + v.x;
    if (new_x >= LEFT_BOUND && (new_x + rec.width) <= RIGHT_BOUND)
        rec.x = new_x;

    float new_y = rec.y + v.y;
    if (new_y >= UPPER_BOUND && (new_y + rec.height) <= LOWER_BOUND)
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
        Vector2 player_ball_dir = normalize_vector({dx, dy});
        direction.x = player_ball_dir.x ? player_ball_dir.x/abs(player_ball_dir.x) : 0;
        direction.y = player_ball_dir.y ? player_ball_dir.y/abs(player_ball_dir.y) : 0;
        set_velocity();

        // use same speed to avoid the curve
        float speed = std::max(abs(velocity.x), abs(velocity.y));
        update_pos({player_ball_dir.x * speed, player_ball_dir.y * speed});
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