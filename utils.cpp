#include <math.h>

#include <raylib.h>


float get_magnitude(const Vector2 v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}


const Vector2 normalize_vector(const Vector2 v)
{
    float magnitude = get_magnitude(v);
    if (magnitude)
        return {v.x/magnitude, v.y/magnitude};

    return {0, 0};
}


float dot_product(const Vector2 &v1, const Vector2 &v2)
{
    return v1.x * v2.x + v1.y * v2.y;
}


void limit_vector(Vector2 &v, float limit)
{
    if (get_magnitude(v) > limit)
    {
        Vector2 norm_vector = normalize_vector(v);
        v.x = norm_vector.x * limit;
        v.y = norm_vector.y * limit;
    }
}