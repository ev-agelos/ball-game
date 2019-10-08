#pragma once
#include <iostream>
#include <tuple>

#include <raylib.h>


std::tuple<float, float> random_pos(int min_x, int max_x, int min_y, int max_y);
void limit_vector(Vector2 &v, float limit);
Vector2 get_nearest_rec_point(Vector2 pos, Rectangle rec);
