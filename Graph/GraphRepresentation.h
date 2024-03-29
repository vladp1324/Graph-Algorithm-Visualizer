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

	bool operator==(const Edge& e) const { return this->idn1 == e.idn1 && this->idn2 == e.idn2; }
};

enum AnimType {
	MARKED,
	VISITED
};

struct edge_for_anim {
	Edge e;
	AnimType type;
};

#define RADIUS 9
#define N_MAX 10 //max nodes