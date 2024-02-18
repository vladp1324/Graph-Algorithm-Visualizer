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
	std::vector < std::pair<int, int> > adj[N_MAX];

	std::vector <edge_for_anim> dfs_anim;
	int nrNotVis;

	const int inf = 2e9;

	void generateNodes();

	void dfs(const int& nc, bool ver[], const Edge& e) const;

	void refreshGraph();

	void generateEdges();

	void dfs_animation(const int& nc, bool vis[], int& time);


public:
	Graph();

	std::vector< Node > getNodes() const;
	
	std::vector< Edge > getEdges() const;

	void setNodes(const std::vector<Node>& nodes);

	void setEdges(const std::vector<Edge>& edges);

	std::vector <edge_for_anim> BFS(const int& source) const;

	std::vector <edge_for_anim> DFS(const int& source);

	std::vector <edge_for_anim> DIJKSTRA(const int& source) const;

	std::vector <edge_for_anim> PRIM(const int& source) const;

	void generateGraph();

	std::vector <olc::vi2d> getNodesPositions(const std::vector <Node>& nodes) const;
};