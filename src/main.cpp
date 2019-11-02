#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "bot.h"
#include "ball.h"
#include "utils.h"
#include "collisions.h"
#include "graphics.h"

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
    InitAudioDevice();
	SetTargetFPS(60);

    // load sounds
    Sound crowd_sound = LoadSound("media/sounds/crowd01.wav");
    SetSoundVolume(crowd_sound, 0.05);
    Sound crowd_goal_sound = LoadSound("media/sounds/crowd02.wav");
    SetSoundVolume(crowd_goal_sound, 0.05);
    Sound ball_sound = LoadSound("media/sounds/ballsound.wav");

    float dt;
    Player p1;
    Bot bot;
    Ball ball(ball_sound);

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
            if (!IsSoundPlaying(crowd_sound))
                PlaySound(crowd_sound);
            dt = GetFrameTime();
            p1.update(dt, ball);
            bot.update(dt, ball, p1);
            ball.update(dt);

            check_collisions(dt, ball, p1, bot);
        }

        draw(p1, bot, ball);

        if (ball.crossed_net)
        {
            PlaySound(crowd_goal_sound);
            reset(p1, bot, ball);
        }
    }

    UnloadSound(crowd_sound);
    UnloadSound(crowd_goal_sound);
    UnloadSound(ball_sound);
    CloseAudioDevice();
	CloseWindow();
	return 0;
}
