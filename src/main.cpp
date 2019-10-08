#include <iostream>
#include "raylib.h"
#include "raymath.h"

#include "player.h"
#include "bot.h"
#include "ball.h"
#include "utils.h"

const int SCREENWIDTH = 800;
const int SCREENHEIGHT = 450;
extern const int TOP_BOUND = 20;
extern const int BOTTOM_BOUND = SCREENHEIGHT - TOP_BOUND;
extern const int LEFT_BOUND = 20;
extern const int RIGHT_BOUND = SCREENWIDTH - LEFT_BOUND;
const int POWERBAR_WIDTH = 100;
const int POWERBAR_HEIGHT = 12;
const int POWERBAR_LR_OFFSET = 50;
const int POWERBAR_BOTTOM_OFFSET = 15;
extern const int GOALPOST_THICKNESS = 10;
extern const float GOALPOST_HEIGHT_START = SCREENHEIGHT / 2 - 50;
extern const float GOALPOST_HEIGHT_END = SCREENHEIGHT / 2 + 50;
extern int LEFT_SCORE = 0;
extern int RIGHT_SCORE = 0;
bool DEBUG = false;
bool PAUSE = false;
int FPS_COUNTER = 0;

void draw(Player &p, Bot &bot, Ball &ball);
void check_collisions(Ball &ball, Player &p, Bot &bot);


void reset(Player &p, Bot &bot, Ball &ball)
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
    Sound crowd_sound = LoadSound("media/sounds/crowd01.wav");
    SetSoundVolume(crowd_sound, 0.05);
    Sound crowd_goal_sound = LoadSound("media/sounds/crowd02.wav");
    SetSoundVolume(crowd_goal_sound, 0.05);

    Player p1;
    Bot bot;
    Ball ball(LoadSound("media/sounds/ballsound.wav"));
    reset(p1, bot, ball);

    while (!WindowShouldClose())
	{
        if (IsKeyPressed(KEY_P))
            PAUSE = !PAUSE;
        if (IsKeyPressed(KEY_R))
            reset(p1, bot, ball);
        if (IsKeyPressed(KEY_F))
            DEBUG = !DEBUG;

        if (!PAUSE)
        {
            if (!IsSoundPlaying(crowd_sound))
                PlaySound(crowd_sound);

            p1.update(ball);
            bot.update(ball, p1);
            ball.update();

            check_collisions(ball, p1, bot);

            if (ball.crossed_net)
            {
                PlaySound(crowd_goal_sound);
                reset(p1, bot, ball);
            }
        }
        else
            FPS_COUNTER++;

        draw(p1, bot, ball);
    }

    UnloadSound(crowd_sound);
    UnloadSound(crowd_goal_sound);
    UnloadSound(ball.kick_sound);
    CloseAudioDevice();
	CloseWindow();
	return 0;
}


void draw(Player & p, Bot & bot, Ball & ball)
{
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
    DrawRectangle(POWERBAR_LR_OFFSET, SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, p.power, POWERBAR_HEIGHT, MAROON);
    DrawRectangleLines(POWERBAR_LR_OFFSET, SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, POWERBAR_WIDTH, POWERBAR_HEIGHT, GRAY);

    DrawRectangle(SCREENWIDTH - (POWERBAR_LR_OFFSET + POWERBAR_WIDTH), SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, POWERBAR_WIDTH, POWERBAR_HEIGHT, LIGHTGRAY);
    DrawRectangle(SCREENWIDTH - (POWERBAR_LR_OFFSET + POWERBAR_WIDTH), SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, bot.power, POWERBAR_HEIGHT, MAROON);
    DrawRectangleLines(SCREENWIDTH - (POWERBAR_LR_OFFSET + POWERBAR_WIDTH), SCREENHEIGHT - POWERBAR_BOTTOM_OFFSET, POWERBAR_WIDTH, POWERBAR_HEIGHT, GRAY);

    // posts
    DrawRectangle(0, GOALPOST_HEIGHT_START, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(0, GOALPOST_HEIGHT_END, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(SCREENWIDTH - 20, GOALPOST_HEIGHT_START, 21, GOALPOST_THICKNESS, GRAY);
    DrawRectangle(SCREENWIDTH - 20, GOALPOST_HEIGHT_END, 21, GOALPOST_THICKNESS, GRAY);

    // field borders
    DrawLineEx(Vector2 {20, 20}, Vector2 {SCREENWIDTH-20, 20}, 1, GRAY);  // top line
    DrawLineEx(Vector2 {20, SCREENHEIGHT - 20}, Vector2 {SCREENWIDTH - 20, SCREENHEIGHT - 20}, 1, GRAY);  // bottom line

    DrawLineEx(Vector2 {20, 20}, Vector2 {20, GOALPOST_HEIGHT_START}, 1, GRAY);  // top left vertical
    DrawLineEx(Vector2 {SCREENWIDTH - 20, 20}, Vector2 {SCREENWIDTH - 20, GOALPOST_HEIGHT_START}, 1, GRAY);  // top right vertical

    DrawLineEx(Vector2 {20, GOALPOST_HEIGHT_END + GOALPOST_THICKNESS}, Vector2 {20, SCREENHEIGHT - 20}, 1, GRAY);  // bottom left vertical
    DrawLineEx(Vector2{SCREENWIDTH - 20, GOALPOST_HEIGHT_END + GOALPOST_THICKNESS}, Vector2{SCREENWIDTH - 20, SCREENHEIGHT - 20}, 1, GRAY);  // bottom right vertical

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


float get_penetration_distance(Rectangle &rec, Vector2 &ball_pos, float &ball_radius)
{
    Vector2 nearest = get_nearest_rec_point(ball_pos, rec);
    float distance = Vector2Distance(ball_pos, nearest);
    return ball_radius - distance;
}


void check_collisions(Ball &ball, Player &p, Bot &bot)
{
    // TODO optimization, references &
    Rectangle p_rec = {p.last_position.x, p.last_position.y, p.rec.width, p.rec.height};
    Vector2 p_direction = Vector2Normalize(p.velocity);

    Vector2 ball_pos = ball.last_position;
    Vector2 ball_direction = Vector2Zero();
    if (ball.velocity.x or ball.velocity.y)
        ball_direction = Vector2Normalize(ball.velocity);

    Vector2 p_velocity = Vector2Zero(), ball_velocity = Vector2Zero();

    while (Vector2Length(p_velocity) < Vector2Length(p.velocity) or Vector2Length(ball_velocity) < Vector2Length(ball.velocity))
    {
        p_velocity = Vector2Add(p_velocity, p_direction);
        if (Vector2Length(p_velocity) > Vector2Length(p.velocity))
            p_velocity = p.velocity;
        ball_velocity = Vector2Add(ball_velocity, ball_direction);
        if (Vector2Length(ball_velocity) > Vector2Length(ball.velocity))
            ball_velocity = ball.velocity;

        p_rec.x = p.last_position.x + p_velocity.x;
        p_rec.y = p.last_position.y + p_velocity.y;
        ball_pos = Vector2Add(ball.last_position, ball_velocity);

        if (CheckCollisionCircleRec(ball_pos, ball.radius, p_rec))
        {
            float penetration_distance = get_penetration_distance(p_rec, ball_pos, ball.radius);
            // make sure distance >= radius + 1 cause raylib casts to int
            if (abs(penetration_distance) < 1)
                penetration_distance = 1;
            Vector2 velocity = Vector2Scale(p_direction, abs(penetration_distance));
            p_velocity = Vector2Subtract(p_velocity, velocity);
            p.handle_collision_response(ball, p_velocity);
            ball.handle_collision_response(p, ball_velocity);
            return;
        }
    }
}