#include <raylib.h>
#include <raymath.h>
#include "ball.h"
#include "player.h"
#include "bot.h"
#include "utils.h"
#include "sounds.h"

extern const int TOP_BOUND;
extern const int BOTTOM_BOUND;
extern const int LEFT_BOUND;
extern const int RIGHT_BOUND;
extern const int GOALPOST_THICKNESS;
extern const float GOALPOST_HEIGHT_START;
extern const float GOALPOST_HEIGHT_END;
extern int LEFT_SCORE;
extern int RIGHT_SCORE;


Ball::Ball()
    :
    friction_c(0.3f),
    acceleration{0, 0},
    radius(5.f),
    position{0, 0},
    velocity{0, 0},
    controlled_by(nullptr),
    crossed_net(false)
{
    setup();
}


void Ball::setup()
{
    position.x = ((RIGHT_BOUND - LEFT_BOUND) / 2.f) + LEFT_BOUND;
    position.y = ((BOTTOM_BOUND - TOP_BOUND) / 2.f) + TOP_BOUND;
    velocity = {0, 0};
    crossed_net = false;
    controlled_by = nullptr;
}


void Ball::set_x(float val)
{
    last_position.x = position.x;
    // ball is in limits
    if (((val - radius) > LEFT_BOUND) && ((val + radius) < RIGHT_BOUND))
    {
        position.x = val;
        return;
    }

    // hit vertical wall else goal
    if (position.y < (GOALPOST_HEIGHT_START + GOALPOST_THICKNESS) || position.y > GOALPOST_HEIGHT_END)
        velocity.x *= -1;
    else
    {
        position.x = val;
        if (!crossed_net)
        {
            if (position.x < LEFT_BOUND)
            {
                RIGHT_SCORE += 1;
                crossed_net = true;
            }
            else if (position.x > RIGHT_BOUND)
            {
                LEFT_SCORE += 1;
                crossed_net = true;
            }
        }
    }
    controlled_by = nullptr;
}


void Ball::set_y(float val)
{
    last_position.y = position.y;

    // ball is in limits
    if (((val - radius) > TOP_BOUND) && ((val + radius) < BOTTOM_BOUND))
    {
        position.y = val;
        return;
    }
    // hit horizontal wall or goal
    velocity.y *= -1;
    controlled_by = nullptr;
}


void Ball::update(float dt)
{
    if (acceleration.x or acceleration.y)
    {
        velocity = Vector2Add(velocity, acceleration);
        acceleration = {0, 0};
    }
    else
    {
        velocity = Vector2Scale(velocity, std::pow(friction_c, dt));
        if (abs(velocity.x) < 0.1)
            velocity.x = 0;
        if (abs(velocity.y) < 0.1)
            velocity.y = 0;
    }

    set_x(position.x + velocity.x * dt);
    set_y(position.y + velocity.y * dt);
}


void Ball::roll(const Vector2& kick_direction, float power)
{
    Vector2 kick_velocity = Vector2Scale(kick_direction, power);
    acceleration = Vector2Subtract(kick_velocity, velocity);
}


void Ball::kick(const Vector2& kick_direction, float power)
{
    roll(kick_direction, power);
    controlled_by = nullptr;
    play_ball_sound();
}


void Ball::handle_collision_response(const Player& p, const Vector2& velocity, float dt)
{
    if (Vector2DotProduct(p.input, Vector2Normalize(p.velocity)) < -0.99)
        return;
    position = last_position;
    this->velocity = velocity;
    set_x(position.x + this->velocity.x * dt);
    set_y(position.y + this->velocity.y * dt);

    if (abs(this->velocity.x) > p.max_speed or abs(this->velocity.y) > p.max_speed)
        this->velocity = Vector2Negate(this->velocity);
}
