#include <raylib.h>
#include <raymath.h>


void limit_vector(Vector2 &v, float limit)
{
    if (Vector2Length(v) > limit)
    {
        Vector2 norm_vector = Vector2Normalize(v);
        v.x = norm_vector.x * limit;
        v.y = norm_vector.y * limit;
    }
}


Vector2 get_nearest_rec_point(Vector2 pos, Rectangle rec)
{
    float nearest_x = Clamp(pos.x, rec.x, rec.x + rec.width);
    float nearest_y = Clamp(pos.y, rec.y, rec.y + rec.height);
    return (Vector2){nearest_x, nearest_y};
}
