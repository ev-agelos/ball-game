#pragma once

#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "bot.h"
#include "ball.h"
#include "utils.h"


const float get_penetration_distance(const Rectangle& rec, const Vector2& ball_pos, const float& ball_radius)
{
    Vector2 nearest = get_nearest_rec_point(ball_pos, rec);
    float distance = Vector2Distance(ball_pos, nearest);
    return ball_radius - distance;
}


void check_collisions(const float& dt, Ball& ball, Player& p, Bot& bot)
{
    int steps = 10;
    Rectangle p_rec = {p.last_position.x, p.last_position.y, p.rec.width, p.rec.height};
    Vector2 p_velocity_step = Vector2Divide(Vector2Scale(p.velocity, dt), steps);

    Vector2 ball_pos = ball.last_position;
    Vector2 ball_velocity_step = Vector2Divide(Vector2Scale(ball.velocity, dt), steps);

    for (int counter=1; counter<=steps; counter++)
    {
        p_rec.x += p_velocity_step.x;
        p_rec.y += p_velocity_step.y;
        ball_pos = Vector2Add(ball_pos, ball_velocity_step);

        if (CheckCollisionCircleRec(ball_pos, ball.radius, p_rec))
        {
            float penetration_distance = get_penetration_distance(p_rec, ball_pos, ball.radius);
            if (penetration_distance < 0)
                continue;

            float total_dt = counter * dt / steps;

            float total_length = Vector2Length(Vector2Scale(p_velocity_step, counter));
            float length = total_length - penetration_distance;
            float time = length * total_dt / total_length;
            Vector2 p_velocity = Vector2Scale(Vector2Normalize(p.velocity), length / time);

            Vector2 ball_velocity = Vector2Scale(ball_velocity_step, counter);

            p.handle_collision_response(ball, p_velocity, time);
            ball.handle_collision_response(p, Vector2Divide(ball_velocity, total_dt), total_dt);
            return;
        }
    }
}
