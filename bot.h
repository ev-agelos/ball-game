#pragma once
#include <raylib.h>
#include "player.h"

class Ball;

class Bot : public Player
{
public:
    Bot(float in_x, float in_y);
    void update(Ball & ball, Player &p);
};