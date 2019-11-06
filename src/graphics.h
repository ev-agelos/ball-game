#pragma once

#include "raylib.h"
#include "raymath.h"
#include "player.h"
#include "bot.h"
#include "ball.h"

extern const int SCREENWIDTH;
extern const int SCREENHEIGHT;
extern bool DEBUG;
extern bool PAUSE;

extern const int TOP_BOUND = 20;
extern const int BOTTOM_BOUND = SCREENHEIGHT - TOP_BOUND;
extern const int LEFT_BOUND = 20;
extern const int RIGHT_BOUND = SCREENWIDTH - LEFT_BOUND;

extern const int GOALPOST_THICKNESS = 10;
extern const float GOALPOST_HEIGHT_START = SCREENHEIGHT / 2 - 50;
extern const float GOALPOST_HEIGHT_END = SCREENHEIGHT / 2 + 50;

static const int POWERBAR_WIDTH = 100;
static const int POWERBAR_HEIGHT = 12;
static const int POWERBAR_LR_OFFSET = 50;
static const int POWERBAR_BOTTOM_OFFSET = 15;
static int FPS_COUNTER;


void draw(const Player& p, const Bot& bot, const Ball& ball, int& left_score, int& right_score)
{
    if (PAUSE)
        FPS_COUNTER++;
    BeginDrawing();
    ClearBackground(BLACK);

    // Score
    DrawText(FormatText("Score: %01i - %01i", left_score, right_score), SCREENWIDTH/2 - 60, SCREENHEIGHT - 18, 19, RED);

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
    DrawRectangle(0, GOALPOST_HEIGHT_START, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(0, GOALPOST_HEIGHT_END, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(SCREENWIDTH - 20, GOALPOST_HEIGHT_START, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(SCREENWIDTH - 20, GOALPOST_HEIGHT_END, 21, GOALPOST_THICKNESS, GRAY);

    // field borders
    DrawLineEx(Vector2{20, 20}, Vector2{SCREENWIDTH - 20.f, 20}, 1, GRAY);  // top line
    DrawLineEx(Vector2{20, SCREENHEIGHT - 20.f}, Vector2{SCREENWIDTH - 20.f, SCREENHEIGHT - 20.f}, 1, GRAY);  // bottom line

    DrawLineEx(Vector2{20, 20}, Vector2{20, GOALPOST_HEIGHT_START}, 1, GRAY);  // top left vertical
    DrawLineEx(Vector2{SCREENWIDTH - 20.f, 20}, Vector2{SCREENWIDTH - 20.f, GOALPOST_HEIGHT_START}, 1, GRAY);  // top right vertical

    DrawLineEx(Vector2{20, GOALPOST_HEIGHT_END + GOALPOST_THICKNESS}, Vector2{20, SCREENHEIGHT - 20.f}, 1, GRAY);  // bottom left vertical
    DrawLineEx(Vector2{SCREENWIDTH - 20.f, GOALPOST_HEIGHT_END + GOALPOST_THICKNESS},Vector2{SCREENWIDTH - 20.f, SCREENHEIGHT - 20.f}, 1, GRAY);  // bottom right vertical

    // FPS
    DrawFPS(1, 1);

    // On pause, we draw a blinking message
    if (PAUSE && ((FPS_COUNTER / 30) % 2))
        DrawText("PAUSED", 350, 200, 30, GRAY);

    // Forces for debugging purposes
    if (DEBUG)
    {
        DrawCircle(p.rec.x + p.rec.width / 2, p.rec.y + p.rec.height / 2, 2, RED);
        DrawCircle(ball.position.x, ball.position.y, 2, RED);
        float mag = Vector2Length(ball.velocity) * 2;
        DrawLineV(ball.position, {ball.position.x + (mag * ball.velocity.x), ball.position.y + (mag * ball.velocity.y)}, RED);
    }

    EndDrawing();
}
