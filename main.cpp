#include <iostream>
#include <tuple>
#include <math.h>
#include "raylib.h"

#include "player.h"
#include "bot.h"
#include "ball.h"

extern const int SCREENWIDTH = 800;
extern const int SCREENHEIGHT = 450;

void draw(Player & p, Bot & bot, Ball & ball);
std::tuple<float, float> random_pos(int min_x, int max_x);


int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Passing Game");
    InitAudioDevice();
	SetTargetFPS(60);
    Sound kick_sound = LoadSound("media/sounds/ballsound.wav");

    auto [player_x, player_y] = random_pos(0, SCREENWIDTH/2);
    auto [bot_x, bot_y] = random_pos(SCREENWIDTH/2, SCREENWIDTH);
    Player p1(player_x, player_y);
    Bot bot(bot_x, bot_y);
    Ball ball;
    ball.kick_sound = kick_sound;
	while (!WindowShouldClose())
	{
        p1.update(ball);
        bot.update(ball, p1);

        ball.check_collision(p1, bot);
        ball.update(p1, bot);

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
    DrawRectangleRec(p.rec, GREEN);
    DrawRectangleRec(bot.rec, BLUE);
    DrawCircleV(ball.position, ball.radius, RAYWHITE);

    // power bars
    DrawRectangle(50, SCREENHEIGHT - 30, 100, 12, LIGHTGRAY);
    DrawRectangle(50, SCREENHEIGHT - 30, p.power, 12, MAROON);
    DrawRectangleLines(50, SCREENHEIGHT - 30, 100, 12, GRAY);

    DrawRectangle(SCREENWIDTH - (30 + 100), SCREENHEIGHT - 30, 100, 12, LIGHTGRAY);
    DrawRectangle(SCREENWIDTH - (30 + 100), SCREENHEIGHT - 30, bot.power, 12, MAROON);
    DrawRectangleLines(SCREENWIDTH - (30 + 100), SCREENHEIGHT - 30, 100, 12, GRAY);
    EndDrawing();
}


std::tuple<float, float> random_pos(int min_x, int max_x)
{
    int x = GetRandomValue(min_x, max_x / 2);
    int y = GetRandomValue(0, SCREENHEIGHT - 20);  // fix this
    return std::make_tuple((float)x, (float)y);
}