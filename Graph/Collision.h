#pragma once
#include "olcPixelGameEngine.h"

bool checkCollisionCircles(const olc::vi2d& c1, const olc::vi2d& c2, const int& nrCircles);

bool checkCollisionLineCircle(const olc::vi2d& P1, const olc::vi2d& P2, const olc::vi2d& C);

bool checkCollisionPointRect(const olc::vi2d& p, const olc::vi2d& rect, const int32_t& width, const int32_t& height);

bool checkCollisionPointCircle(const olc::vi2d& p, const olc::vi2d& c);

bool checkCollisionCircleVectorCircles(const olc::vi2d& c, const std::vector<olc::vi2d>& vc);

bool simulateLineCollision(const olc::vi2d& p1, const olc::vi2d& p2, const std::vector<olc::vi2d>& vc);