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