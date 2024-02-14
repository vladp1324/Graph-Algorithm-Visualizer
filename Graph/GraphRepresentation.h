#pragma once
#include "olcPixelGameEngine.h"
#include <cstdint>
#include <cstdlib>

struct Node {
	int id;
	olc::vi2d pos;
};

struct Edge {
	int idn1;
	int idn2;
	int cost = 1 + rand() % 15;
};

struct edge_for_anim {
	Edge e;
	int dist;
};