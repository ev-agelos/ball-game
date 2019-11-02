#pragma once

#include <raylib.h>
#include "player.h"
#include "ball.h"


class Bot : public Player
{
public:
    Bot() : Player(3.f){ setup(); };
    void update(float& dt, Ball& ball, Player& p){};
    void setup();
};
