#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "bot.h"
#include "ball.h"
#include "utils.h"
#include "collisions.h"
#include "graphics.h"
#include "sounds.h"

extern const int SCREENWIDTH = 800;
extern const int SCREENHEIGHT = 450;
bool DEBUG = false;
bool PAUSE = false;
bool IS_GOAL = false;


void reset(Player& p, Bot& bot, Ball& ball)
{
    p.setup();
    bot.setup();
    ball.setup();
}


void read_user_input(Player& p1, Bot& bot, Ball& ball)
{
    if (IsKeyPressed(KEY_P))
        PAUSE = !PAUSE;
    if (IsKeyPressed(KEY_R))
        reset(p1, bot, ball);
    if (IsKeyPressed(KEY_F))
        DEBUG = !DEBUG;
    if (IsKeyDown(KEY_D) and p1.power < p1.max_power)
        p1.power += 100;
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
        read_user_input(p1, bot, ball);

        if (not PAUSE)
        {
            dt = GetFrameTime();
            p1.update(dt, ball);
            bot.update(dt, ball);
            ball.update(dt);

            check_collisions(dt, ball, p1);
        }
        draw(p1, bot, ball);
        play_sounds();

        if (IS_GOAL)
        {
            reset(p1, bot, ball);
            IS_GOAL = false;
        }
    }

    unload_sounds();
	CloseWindow();
	return 0;
}
