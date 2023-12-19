#pragma once
#include "olcPixelGameEngine.h"

bool checkCollisionCircles(olc::vd2d c1, olc::vd2d c2);

bool checkCollisionLineCircle(olc::vd2d P1, olc::vd2d P2, olc::vd2d C);

bool checkCollisionPointRect(olc::vd2d p, olc::vd2d rect, int32_t width, int32_t height);

bool checkCollisionPointCircle(olc::vd2d p, olc::vd2d c);