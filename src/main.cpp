#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "bot.h"
#include "ball.h"
#include "utils.h"
#include "collisions.h"
#include "graphics.h"
#include "sounds.h"

extern Music BG_MUSIC;
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
    PlayMusicStream(BG_MUSIC);

    float dt;
    Player p1;
    Bot bot;
    Ball ball;

    while (!WindowShouldClose())
	{
        UpdateMusicStream(BG_MUSIC);
        read_user_input(p1, bot, ball);

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
