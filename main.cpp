#include <iostream>
#include <tuple>
#include <math.h>
#include "raylib.h"

#include "player.h"
#include "bot.h"
#include "ball.h"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
extern const int UPPER_BOUND = 20;
extern const int LOWER_BOUND = SCREENHEIGHT - UPPER_BOUND;
extern const int LEFT_BOUND = 20;
extern const int RIGHT_BOUND = SCREENWIDTH - LEFT_BOUND;
extern const int GOALPOST_THICKNESS = 10;
extern const float GOALPOST_HEIGHT_START = SCREENHEIGHT / 2 - 50;
extern const float GOALPOST_HEIGHT_END = SCREENHEIGHT / 2 + 50;

void draw(Player & p, Bot & bot, Ball & ball);
std::tuple<float, float> random_pos(int min_x, int max_x);


int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Passing Game");
    InitAudioDevice();
	SetTargetFPS(60);
    Sound kick_sound = LoadSound("media/sounds/ballsound.wav");

    auto [player_x, player_y] = random_pos(LEFT_BOUND, ((RIGHT_BOUND - LEFT_BOUND) / 2) + LEFT_BOUND);
    auto [bot_x, bot_y] = random_pos(RIGHT_BOUND - ((RIGHT_BOUND - LEFT_BOUND) / 2), RIGHT_BOUND);
    Player p1(player_x, player_y);
    Bot bot(bot_x, bot_y);
    Ball ball;
    ball.kick_sound = kick_sound;
	while (!WindowShouldClose())
	{
        p1.update(ball);
        bot.update(ball, p1);

        ball.check_collision(p1, bot);
        ball.update();

        draw(p1, bot, ball);
	}

    UnloadSound(kick_sound);
    CloseAudioDevice();
	CloseWindow();
	return 0;
}


void draw(Player & p, Bot & bot, Ball & ball)
{
    BeginDrawing();
    ClearBackground(BLACK);

    // players and ball
    DrawRectangleRec(p.rec, GREEN);
    DrawRectangleRec(bot.rec, BLUE);
    DrawCircleV(ball.position, ball.radius, RAYWHITE);

    // power bars
    DrawRectangle(50, SCREENHEIGHT - 15, 100, 12, LIGHTGRAY);
    DrawRectangle(50, SCREENHEIGHT - 15, p.power, 12, MAROON);
    DrawRectangleLines(50, SCREENHEIGHT - 15, 100, 12, GRAY);

    DrawRectangle(SCREENWIDTH - (30 + 100), SCREENHEIGHT - 15, 100, 12, LIGHTGRAY);
    DrawRectangle(SCREENWIDTH - (30 + 100), SCREENHEIGHT - 15, bot.power, 12, MAROON);
    DrawRectangleLines(SCREENWIDTH - (30 + 100), SCREENHEIGHT - 15, 100, 12, GRAY);

    // posts
    DrawRectangle(0, GOALPOST_HEIGHT_START, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(0, GOALPOST_HEIGHT_END, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(SCREENWIDTH - 22, GOALPOST_HEIGHT_START, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(SCREENWIDTH - 22, GOALPOST_HEIGHT_END, 21, GOALPOST_THICKNESS, GRAY);

    // field borders
    DrawLineEx(Vector2 {20, 20}, Vector2 {SCREENWIDTH-20, 20}, 3, GRAY);
    DrawLineEx(Vector2 {20, SCREENHEIGHT - 20}, Vector2 {SCREENWIDTH - 20, SCREENHEIGHT - 20}, 3, GRAY);

    DrawLineEx(Vector2 {20, 20}, Vector2 {20, GOALPOST_HEIGHT_START}, 3, GRAY);
    DrawLineEx(Vector2 {SCREENWIDTH - 20, 20}, Vector2 {SCREENWIDTH - 20, GOALPOST_HEIGHT_START}, 3, GRAY);

    DrawLineEx(Vector2 {20, GOALPOST_HEIGHT_END + GOALPOST_THICKNESS}, Vector2 {20, SCREENHEIGHT - 20}, 3, GRAY);
    DrawLineEx(Vector2 {SCREENWIDTH - 20, GOALPOST_HEIGHT_END + GOALPOST_THICKNESS}, Vector2 {SCREENWIDTH - 20, SCREENHEIGHT - 20}, 3, GRAY);

    EndDrawing();
}


std::tuple<float, float> random_pos(int min_x, int max_x)
{
    int x = GetRandomValue(min_x, max_x / 2);
    int y = GetRandomValue(UPPER_BOUND, LOWER_BOUND);
    return std::make_tuple((float)x, (float)y);
}