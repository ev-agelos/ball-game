#pragma once

#include <raylib.h>

#include "player.hpp"
#include "ball.hpp"


class Bot : public Player
{
public:
    Bot() : Player(3.f){ setup(); };
    void setup();
};
