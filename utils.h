#pragma once
#include <iostream>
#include <tuple>

#include <raylib.h>


float get_magnitude(const Vector2 v);
Vector2 normalize_vector(const Vector2 v);
float dot_product(const Vector2 &v1, const Vector2 &v2);
std::tuple<float, float> random_pos(int min_x, int max_x, int min_y, int max_y);
void limit_vector(Vector2 &v, float limit);