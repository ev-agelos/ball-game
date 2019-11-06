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
bool IS_GOAL = false;

extern const int SCREENWIDTH = 800;
extern const int SCREENHEIGHT = 450;


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

    int left_score = 0;
    int right_score = 0;
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

        if (IS_GOAL)
        {
            play_crowd_cheering();
            if (ball.position.x < SCREENWIDTH / 2)
                right_score += 1;
            else
                left_score += 1;
            reset(p1, bot, ball);
            IS_GOAL = false;
        }

        draw(p1, bot, ball, left_score, right_score);
    }

    unload_sounds();
	CloseWindow();
	return 0;
}
