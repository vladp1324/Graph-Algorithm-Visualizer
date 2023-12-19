#pragma once
#include "olcPixelGameEngine.h"
#include "GraphRepresentation.h"
#include <queue>

#define N_MAX 10 //max nodes

class Graph
{
private:
	int nrNodes;
	int nrEdges;

	std::vector< Edge > edges;
	std::vector< Node > nodes;
	std::vector < Node > v[N_MAX];

	bool simulateLineCollision(olc::vd2d p1, olc::vd2d p2);

	void generateNodes();

	void dfs(int nc, bool ver[], Edge e);

	void refreshGraph();

	void generateEdges();
	
	std::vector <edge_for_anim> dfs_anim;
	int nrNotVis;

	void dfs_animation(int nc, bool vis[], int& time);

	const int inf = 2e9;

public:
	Graph();

	std::vector< Node > getNodes();
	
	std::vector< Edge > getEdges();

	std::vector <edge_for_anim> BFS(int source);

	std::vector <edge_for_anim> DFS(int source);

	std::vector <edge_for_anim> DIJKSTRA(int source);

	void generateGraph();
};