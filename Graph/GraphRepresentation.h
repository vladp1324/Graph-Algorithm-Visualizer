#pragma once
#include <cstdint>
#include <cstdlib>
struct Node {
	int id;
	int32_t x;
	int32_t y;
};

struct Edge {
	Node n1;
	Node n2;
	int cost = 1 + rand() % 15;
};

struct edge_for_anim {
	Edge e;
	int dist;
};