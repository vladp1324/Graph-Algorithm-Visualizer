#include "Graph.h"
#include "Collision.h"

bool Graph::simulateLineCollision(olc::vi2d p1, olc::vi2d p2)
{
	for (int i = 0; i < nodes.size(); i++) {
		if (olc::vi2d(p1) != olc::vi2d(nodes[i].pos.x, nodes[i].pos.y) && olc::vi2d(p2) != olc::vi2d(nodes[i].pos.x, nodes[i].pos.y)
			&& checkCollisionLineCircle(olc::vi2d(p1.x, p1.y), olc::vi2d(p2.x, p2.y), olc::vi2d(nodes[i].pos.x, nodes[i].pos.y))) {
			return true;
		}
	}

	return false;
}

void Graph::generateNodes()
{
	nodes.clear();
	int nrMinNodes = 4;
	nrNodes = nrMinNodes + rand() % (N_MAX - nrMinNodes + 1); //random: 5-10
	//nrNodes = 7;

	while (nodes.size() < nrNodes)
	{
		int32_t x = 20 + rand() % 271;
		int32_t y = 20 + rand() % 221;
		bool foundPosition = true;

		for (int i = 0; i < nodes.size(); i++) {
			if (checkCollisionCircles(olc::vi2d(x, y), nodes[i].pos, 6) || simulateLineCollision(olc::vi2d(x, y), nodes[i].pos)) {
				foundPosition = false;
				break;
			}
		}

		if (foundPosition) {
			nodes.push_back({ (int)nodes.size(), {x, y} });
		}
	}
}

void Graph::dfs(int nc, bool ver[], Edge e)
{
	for (int i = 0; i < v[nc].size(); i++) {
		int nv = v[nc][i].id;

		if ((nc == e.idn1 && nv == e.idn2) ||
			(nc == e.idn2 && nv == e.idn1)
			) {
			//nu folosi muchia
			continue;
		}

		if (not ver[nv]) {
			ver[nv] = true;
			dfs(nv, ver, e);
		}
	}
}

void Graph::refreshGraph()
{
	for (int i = 0; i < nrNodes; i++)
		v[i].clear();

	for (auto e : edges) {
		v[e.idn1].push_back(nodes[e.idn2]);
		v[e.idn2].push_back(nodes[e.idn1]);
	}
}

void Graph::generateEdges()
{
	edges.clear();
	//int degree[N_MAX];
	//std::fill(degree, degree + N_MAX, 0);
	//nrEdges = nrNodes - 1 + rand() % (nrNodes - 1);
	//nrEdges = nrNodes - 1 + (nrNodes - 1);

	//generate 1->(n-1) (n-1 edges)
	/*for (int i = 0; i < nrNodes - 1; i++) {
		edges.push_back(Edge(nodes[nrNodes - 1], nodes[i]));
	}*/

	//generate 1->(1-2) ( n - 1 edges)
	/*for (int i = 0; i < nrNodes; i++) {
		if (degree[i] == 0) {
			for (int j = 0; j < nrNodes; j++) {
				if (i != j && degree[j] != 2) {
					degree[i]++;
					degree[j]++;
					edges.push_back(Edge(nodes[i], nodes[j]));
					break;
				}
			}
		}
	}*/
	//generate random edges
	/*while (edges.size() < nrEdges) {
		int n1 = rand() % nrNodes;
		int n2 = rand() % nrNodes;
		while (n1 == n2) {
			int n2 = rand() % nrNodes;
		}
		bool foundEdge = true;
		if (foundEdge) {
			edges.push_back(Edge(nodes[n1], nodes[n2]));
		}
	}*/

	//generate i->(n-1 - i) (n * (n-1) / 2 edges)
	for (int i = 0; i < nrNodes; i++) {
		for (int j = i + 1; j < nrNodes; j++) {
			edges.push_back({ i, j });
		}
	}

	//delete all edges that collide with a node
	std::vector< Edge > edgesCopy;

	for (auto e : edges) {
		if (!simulateLineCollision(nodes[e.idn1].pos, nodes[e.idn2].pos))
			edgesCopy.push_back(e);
	}

	edges = edgesCopy;
	refreshGraph();

	//delete random edges
	int nrMaxDeleteEdges = edges.size() - nrNodes + 1;
	int nrMinDeleteEdges = 1;
	int nrDeletedEdges = nrMinDeleteEdges + rand() % (nrMaxDeleteEdges - nrMinDeleteEdges + 1) - 1;
	//int nrDeletedEdges = nrMaxDeleteEdges;
	//nrEdges = edges.size() - nrDeletedEdges;
	nrEdges = nrNodes + rand() % (nrNodes / 2);

	while (nrEdges < edges.size()) {
		edgesCopy.clear();
		int randomEdge = rand() % edges.size();
		bool ver[N_MAX];
		std::fill(ver, ver + N_MAX, 0);

		dfs(edges[randomEdge].idn1, ver, edges[randomEdge]);
		if (ver[edges[randomEdge].idn2]) {
			for (int i = 0; i < edges.size(); i++) {
				if (randomEdge != i) {
					edgesCopy.push_back(edges[i]);
				}
			}

			edges = edgesCopy;
			refreshGraph(); //se poate mai eficient
		}
	}
}



std::vector<edge_for_anim> Graph::BFS(int source)
{
	int d[N_MAX];
	int prev[N_MAX];
	std::fill(d, d + N_MAX, -1);
	std::fill(prev, prev + N_MAX, -1);

	std::queue<int> q;
	std::vector<edge_for_anim> ans;

	d[source] = 0;
	q.push(source);


	while (!q.empty()) {
		int nc = q.front();
		q.pop();
		//std::cout << nc << '\n';

		for (auto e : edges) {
			if (e.idn1 != nc && e.idn2 != nc)
				continue;

			Node nv = nodes[e.idn2];

			if (e.idn2 == nc)
				nv = nodes[e.idn1];

			if (d[nv.id] == -1) {
				d[nv.id] = d[nc] + 1;
				q.push(nv.id);
				ans.push_back(edge_for_anim{ {nc, nv.id}, d[nv.id] });
			}

		}
	}

	return ans;
}

void Graph::dfs_animation(int nc, bool vis[], int& time)
{
	if (nrNotVis == 0)
		return;

	vis[nc] = true;
	nrNotVis--;

	for (auto e : edges) {
		if (e.idn1 != nc && e.idn2 != nc)
			continue;

		Node nv = nodes[e.idn2];

		if (e.idn2 == nc)
			nv = nodes[e.idn1];

		if (not vis[nv.id]) {
			time++;
			dfs_anim.push_back(edge_for_anim{ { nc, nv.id }, time });
			dfs_animation(nv.id, vis, time);

			if (nrNotVis > 0) {
				time++;
				dfs_anim.push_back(edge_for_anim{ { nv.id, nc }, time });
			}
		}
	}
}

std::vector<edge_for_anim> Graph::DFS(int source)
{
	dfs_anim.clear();
	bool vis[N_MAX];
	std::fill(vis, vis + N_MAX, false);
	int time = 0;
	nrNotVis = nodes.size();

	dfs_animation(source, vis, time);

	return dfs_anim;
}

std::vector<edge_for_anim> Graph::DIJKSTRA(int source)
{
	bool vis[N_MAX];
	std::fill(vis, vis + N_MAX, false);

	int d[N_MAX];
	std::fill(d, d + N_MAX, inf);
	d[source] = 0;

	std::vector<edge_for_anim> ans;

	std::priority_queue < std::pair < int, std::pair <int, int> > > pq;
	pq.push({ 0, {source, source} });
	while (!pq.empty()) {
		int nc = pq.top().second.first;
		int prev = pq.top().second.second;
		pq.pop();

		if (!vis[nc]) {
			for (auto e : edges) {
				if (e.idn1 != nc && e.idn2 != nc)
					continue;

				int cnv = e.cost;
				int nv = e.idn2;
				if (e.idn2 == nc)
					nv = e.idn1;

				if (cnv + d[nc] < d[nv]) {
					d[nv] = cnv + d[nc];
					pq.push({ -d[nv], {nv, nc} });
				}
			}

			vis[nc] = true;
			ans.push_back(edge_for_anim{ { prev, nc}, d[nc] });
		}

	}

	ans.erase(ans.begin());

	return ans;
}

void Graph::generateGraph()
{
	srand(static_cast<unsigned>(time(NULL)));
	generateNodes();
	generateEdges();
}

Graph::Graph()
{
}

std::vector<Node> Graph::getNodes()
{
	return this->nodes;
}

std::vector<Edge> Graph::getEdges()
{
	return this->edges;
}

void Graph::setNodes(const std::vector<Node>& nodes)
{
	this->nodes = nodes;
}

void Graph::setEdges(const std::vector<Edge>& edges)
{
	this->edges = edges;
}