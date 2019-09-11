#include <math.h>

#include "utils.h"


Vector2 normalize_vector(const Vector2 v)
{
    float magnitude = sqrt(v.x * v.x + v.y * v.y);
    if (magnitude)
        return {v.x/magnitude, v.y/magnitude};

    return {0, 0};
}


float dot_product(const Vector2 &v1, const Vector2 &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}


std::tuple<float, float> random_pos(int min_x, int max_x, int min_y, int max_y)
{
    int x = GetRandomValue(min_x, max_x / 2);
    int y = GetRandomValue(min_y, max_y);
    return std::make_tuple((float)x, (float)y);
}