#include "bot.h"

extern const int LEFT_BOUND;
extern const int RIGHT_BOUND;
extern const int TOP_BOUND;
extern const int BOTTOM_BOUND;


void Bot::setup()
{
    rec.x = GetRandomValue(((RIGHT_BOUND - LEFT_BOUND) / 2) + LEFT_BOUND, RIGHT_BOUND - rec.width);
    rec.y = GetRandomValue(TOP_BOUND, BOTTOM_BOUND - rec.height);
    velocity = {0, 0};
    power = 0;
}
