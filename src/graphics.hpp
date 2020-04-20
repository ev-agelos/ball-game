#pragma once

#include <string>

#include "raylib.h"
#include "raymath.h"

#include "player.hpp"
#include "bot.hpp"
#include "ball.hpp"

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;
extern const int APPROACH_RADIUS;
extern bool IS_GOAL;
extern bool DEBUG;
extern bool PAUSE;

Rectangle FIELD = {20, 20, SCREENWIDTH - 40.f, SCREENHEIGHT - 40.f};
Rectangle LEFT_NET = {0, SCREENHEIGHT / 2.f - 50, 20, 100};
Rectangle RIGHT_NET = {SCREENWIDTH - 20.f, SCREENHEIGHT / 2.f - 50, 20, 100};

const int GOALPOST_THICKNESS = 10;
static const int POWERBAR_WIDTH = 100;
static const int POWERBAR_HEIGHT = 12;
static const int POWERBAR_LR_OFFSET = 50;
static const int POWERBAR_BOTTOM_OFFSET = 15;
static int FPS_COUNTER;
static int LEFT_SCORE = 0;
static int RIGHT_SCORE = 0;


void draw(const Player& p, const Bot& bot, const Ball& ball)
{
    if (PAUSE)
        FPS_COUNTER++;

    if (IS_GOAL)
    {
        if (ball.position.x < SCREENWIDTH / 2)
            RIGHT_SCORE += 1;
        else
            LEFT_SCORE += 1;
    }

    BeginDrawing();
    ClearBackground(BLACK);

    // Score
    DrawText(FormatText("Score: %01i - %01i", LEFT_SCORE, RIGHT_SCORE), SCREENWIDTH/2 - 60, SCREENHEIGHT - 18, 19, RED);

    // players and ball
    DrawRectangleRec(p.rec, GREEN);
    DrawRectangleRec(bot.rec, BLUE);
    DrawCircleV(ball.position, ball.radius, RAYWHITE);

    // power bars
    DrawRectangle(POWERBAR_LR_OFFSET, SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, POWERBAR_WIDTH, POWERBAR_HEIGHT, LIGHTGRAY);
    DrawRectangle(POWERBAR_LR_OFFSET, SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, p.power * 100 / p.max_power, POWERBAR_HEIGHT, MAROON);
    DrawRectangleLines(POWERBAR_LR_OFFSET, SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, POWERBAR_WIDTH, POWERBAR_HEIGHT, GRAY);

    DrawRectangle(SCREENWIDTH - (POWERBAR_LR_OFFSET + POWERBAR_WIDTH), SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, POWERBAR_WIDTH, POWERBAR_HEIGHT, LIGHTGRAY);
    DrawRectangle(SCREENWIDTH - (POWERBAR_LR_OFFSET + POWERBAR_WIDTH), SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, bot.power * 100/ bot.max_power, POWERBAR_HEIGHT, MAROON);
    DrawRectangleLines(SCREENWIDTH - (POWERBAR_LR_OFFSET + POWERBAR_WIDTH), SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, POWERBAR_WIDTH, POWERBAR_HEIGHT, GRAY);

    // posts
    DrawRectangle(0, LEFT_NET.y - GOALPOST_THICKNESS, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(0, LEFT_NET.y + LEFT_NET.height, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(RIGHT_NET.x, RIGHT_NET.y - GOALPOST_THICKNESS, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(RIGHT_NET.x, RIGHT_NET.y + RIGHT_NET.height, 21, GOALPOST_THICKNESS, GRAY);

    // field borders
    DrawLineEx(Vector2{20, 20}, Vector2{SCREENWIDTH - 20.f, 20}, 1, GRAY);  // top line
    DrawLineEx(Vector2{20, SCREENHEIGHT - 20.f}, Vector2{SCREENWIDTH - 20.f, SCREENHEIGHT - 20.f}, 1, GRAY);  // bottom line

    DrawLineEx(Vector2{FIELD.x, FIELD.y}, Vector2{FIELD.x, LEFT_NET.y}, 1, GRAY);  // top left vertical
    DrawLineEx(Vector2{FIELD.x + FIELD.width, FIELD.y}, Vector2{RIGHT_NET.x, RIGHT_NET.y}, 1, GRAY);  // top right vertical

    DrawLineEx(Vector2{FIELD.x, LEFT_NET.y + LEFT_NET.height}, Vector2{FIELD.x, FIELD.y + FIELD.height}, 1, GRAY);  // bottom left vertical
    DrawLineEx(Vector2{RIGHT_NET.x, RIGHT_NET.y + RIGHT_NET.height}, Vector2{RIGHT_NET.x, FIELD.y + FIELD.height}, 1, GRAY);  // bottom right vertical

    // FPS
    DrawFPS(1, 1);

    // On pause, we draw a blinking message
    if (PAUSE && ((FPS_COUNTER / 30) % 2))
        DrawText("PAUSED", 350, 200, 30, GRAY);

    // Forces for debugging purposes
    if (DEBUG)
    {
        DrawCircleLines(ball.position.x, ball.position.y, APPROACH_RADIUS + ball.radius, WHITE);
        // Acceleration bar
        DrawRectangleLines(40, SCREENHEIGHT - 50.f, p.max_speed, 10, WHITE);
        DrawRectangle(40, SCREENHEIGHT - 50.f, Vector2Length(p.acceleration), 10, GREEN);
        DrawText(("Acc: " + std::to_string(Vector2Length(p.acceleration))).c_str(), 40 + p.max_speed, SCREENHEIGHT - 50.f, 10, GREEN);

        // Velocity bar
        DrawRectangleLines(40, SCREENHEIGHT - 40.f, p.max_speed, 10, WHITE);
        DrawRectangle(40, SCREENHEIGHT - 40.f, Vector2Length(p.velocity), 10, GREEN);
        DrawText(("Vel: " + std::to_string(Vector2Length(p.velocity))).c_str(), 40 + p.max_speed, SCREENHEIGHT - 40.f, 10, GREEN);

        DrawCircle(p.rec.x + p.rec.width / 2, p.rec.y + p.rec.height / 2, 2, RED);
        DrawCircle(ball.position.x, ball.position.y, 2, RED);
        float p_mag = Vector2Length(p.input) * 2;
        DrawLineV({p.rec.x + p.rec.width / 2, p.rec.y + p.rec.height/2}, {p.rec.x + p.rec.width / 2 + (p_mag * p.velocity.x), p.rec.y + p.rec.height / 2 + (p_mag * p.velocity.y)}, RED);


        DrawText(("Ball Vel " + std::to_string(int(Vector2Length(ball.velocity)))).c_str(), SCREENWIDTH / 2.f - 10, SCREENHEIGHT - 40.f, 15, GREEN);
        float mag = Vector2Length(ball.velocity) * 2;
        DrawLineV(ball.position, {ball.position.x + (mag * ball.velocity.x), ball.position.y + (mag * ball.velocity.y)}, RED);
    }

    EndDrawing();
}
