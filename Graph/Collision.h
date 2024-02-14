#pragma once
#include "olcPixelGameEngine.h"

bool checkCollisionCircles(olc::vi2d c1, olc::vi2d c2, int nrCircles);

bool checkCollisionLineCircle(olc::vi2d P1, olc::vi2d P2, olc::vi2d C);

bool checkCollisionPointRect(olc::vi2d p, olc::vi2d rect, int32_t width, int32_t height);

bool checkCollisionPointCircle(olc::vi2d p, olc::vi2d c);

bool checkCollisionCircleVectorCircles(olc::vi2d c, std::vector<olc::vi2d> vc);