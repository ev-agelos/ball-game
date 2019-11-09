#include "bot.hpp"

extern Rectangle FIELD;
extern Rectangle RIGHT_NET;



void Bot::setup()
{
    rec.x = GetRandomValue(FIELD.width / 2 + FIELD.x, RIGHT_NET.x - rec.width);
    rec.y = GetRandomValue(FIELD.y, FIELD.y + FIELD.height - rec.height);
    velocity = {0, 0};
    power = 0;
}
