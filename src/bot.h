#pragma once
#include <raylib.h>
#include "player.h"

class Ball;

class Bot : public Player
{
public:
    Bot();
    void update(Ball & ball, Player &p);
    void setup();
};