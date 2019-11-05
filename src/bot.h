#pragma once

#include <raylib.h>
#include "player.h"
#include "ball.h"


class Bot : public Player
{
public:
    Bot() : Player(3.f){ setup(); };
    void setup();
};
