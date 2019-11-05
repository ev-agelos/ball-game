#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "bot.h"
#include "ball.h"
#include "utils.h"
#include "collisions.h"
#include "graphics.h"
#include "sounds.h"

bool DEBUG = false;
bool PAUSE = false;
int FPS_COUNTER = 0;

extern const int SCREENWIDTH = 800;
extern const int SCREENHEIGHT = 450;
int LEFT_SCORE = 0;
int RIGHT_SCORE = 0;


void reset(Player& p, Bot& bot, Ball& ball)
{
    p.setup();
    bot.setup();
    ball.setup();
}


int main()
{
	InitWindow(SCREENWIDTH, SCREENHEIGHT, "Ball Game");
	SetTargetFPS(60);
    load_sounds();

    float dt;
    Player p1;
    Bot bot;
    Ball ball;

    while (!WindowShouldClose())
	{
        if (IsKeyPressed(KEY_P))
            PAUSE = !PAUSE;
        if (IsKeyPressed(KEY_R))
            reset(p1, bot, ball);
        if (IsKeyPressed(KEY_F))
            DEBUG = !DEBUG;

        if (PAUSE)
            FPS_COUNTER++;
        else
        {
            play_crowd_sound();
            dt = GetFrameTime();
            p1.update(dt, ball);
            bot.update(dt, ball);
            ball.update(dt);

            check_collisions(dt, ball, p1);
        }

        draw(p1, bot, ball);

        if (ball.crossed_net)
        {
            play_crowd_cheering();
            reset(p1, bot, ball);
        }
    }

    unload_sounds();
	CloseWindow();
	return 0;
}
