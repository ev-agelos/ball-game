#pragma once

#include <raylib.h>
#include <raymath.h>


inline const Vector2 limit_vector(const Vector2& v, float limit)
{
    if (Vector2Length(v) > limit)
        return Vector2Scale(Vector2Normalize(v), limit);
    else
        return v;
}


inline const Vector2 get_nearest_rec_point(const Vector2& pos, const Rectangle& rec)
{
    float nearest_x = Clamp(pos.x, rec.x, rec.x + rec.width);
    float nearest_y = Clamp(pos.y, rec.y, rec.y + rec.height);
    return Vector2{nearest_x, nearest_y};
}
