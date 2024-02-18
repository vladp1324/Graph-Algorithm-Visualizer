#include "Graph.h"
#include "Collision.h"

void Graph::generateNodes()
{
	nodes.clear();
	int nrMinNodes = 4;
	nrNodes = nrMinNodes + rand() % (N_MAX - nrMinNodes + 1); //random: 5-10

	while (nodes.size() < nrNodes)
	{
		int32_t x = 20 + rand() % 271;
		int32_t y = 20 + rand() % 221;
		bool foundPosition = true;

		for (const auto& node : nodes) {
			if (checkCollisionCircles(olc::vi2d(x, y), node.pos, 6) || simulateLineCollision(olc::vi2d(x, y), node.pos, getNodesPositions(nodes))) {
				foundPosition = false;
				break;
			}
		}

		if (foundPosition) {
			nodes.push_back({ (int)nodes.size(), {x, y} });
		}
	}
}

void Graph::dfs(const int& nc, bool ver[], const Edge& e) const
{
	for (const auto& mc : adj[nc]) {
		int nv = mc.first;
	
		if ((nc == e.idn1 && nv == e.idn2) ||
			(nc == e.idn2 && nv == e.idn1)) {
			//don't use edge
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
		adj[i].clear();

	for (const auto& e : edges) {
		adj[e.idn1].push_back({ e.idn2, e.cost});
		adj[e.idn2].push_back({ e.idn1, e.cost});
	}
}

void Graph::generateEdges()
{
	edges.clear();

	//generate i->(n-1 - i) (n * (n-1) / 2 edges)
	for (int i = 0; i < nrNodes; i++) {
		for (int j = i + 1; j < nrNodes; j++) {
			edges.push_back({ i, j });
		}
	}

	//delete all edges that collide with a node
	std::vector< Edge > edgesCopy;

	for (const auto& e : edges) {
		if (!simulateLineCollision(nodes[e.idn1].pos, nodes[e.idn2].pos, getNodesPositions(nodes)))
			edgesCopy.push_back(e);
	}
	edges = edgesCopy;
	refreshGraph();

	//delete random edges
	int nrMaxDeleteEdges = edges.size() - nrNodes + 1;
	int nrMinDeleteEdges = 1;
	int nrDeletedEdges = nrMinDeleteEdges + rand() % (nrMaxDeleteEdges - nrMinDeleteEdges + 1) - 1;
	nrEdges = nrNodes + rand() % (nrNodes / 2);
	
	while (nrEdges < edges.size()) {
		int randomEdge = rand() % edges.size();
		bool ver[N_MAX];
		std::fill(ver, ver + N_MAX, 0);

		dfs(edges[randomEdge].idn1, ver, edges[randomEdge]);
		if (ver[edges[randomEdge].idn2]) {
			edges.erase(edges.begin() + randomEdge);

			refreshGraph(); //se poate mai eficient
		}
	}
	
	refreshGraph();
}



std::vector<edge_for_anim> Graph::BFS(const int& source) const
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

		for(const auto& e : adj[nc]) {
			int nv = e.first;

			if (d[nv] == -1) {
				d[nv] = d[nc] + 1;
				q.push(nv);
				ans.push_back(edge_for_anim{ {nc, nv}, d[nv] });
			}

		}
	}

	return ans;
}

void Graph::dfs_animation(const int& nc, bool vis[], int& time)
{
	if (nrNotVis == 0)
		return;

	vis[nc] = true;
	nrNotVis--;

	for(const auto& e : adj[nc]){
		int nv = e.first;

		if (not vis[nv]) {
			time++;
			dfs_anim.push_back(edge_for_anim{ { nc, nv }, time });
			dfs_animation(nv, vis, time);

			if (nrNotVis > 0) {
				time++;
				dfs_anim.push_back(edge_for_anim{ { nv, nc }, time });
			}
		}
	}
}

std::vector<edge_for_anim> Graph::DFS(const int& source)
{
	dfs_anim.clear();
	bool vis[N_MAX];
	std::fill(vis, vis + N_MAX, false);
	int time = 0;
	nrNotVis = nodes.size();

	dfs_animation(source, vis, time);

	return dfs_anim;
}

std::vector<edge_for_anim> Graph::DIJKSTRA(const int& source) const
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
			for(const auto& e : adj[nc]){
				int cnv = e.second;
				int nv = e.first;

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

std::vector<edge_for_anim> Graph::PRIM(const int& source) const
{
	bool vis[N_MAX + 1];
	std::fill(vis, vis + N_MAX, false);

	std::priority_queue < std::pair <int, std::pair <int, int> > > pq; //cost | current node | prev node

	std::vector<edge_for_anim> ans;

	pq.push({ 0, {source, -1} });
	while (!pq.empty()) {
		int nc = pq.top().second.first;
		int nod = pq.top().second.second;
		int c = pq.top().first;
		pq.pop();
		
		if (vis[nc])
			continue;

		if (!vis[nc]) {
			for(const auto& e : adj[nc]){
				int cst = e.second;
				int nv = e.first;

				if (!vis[nv]) {
					pq.push({ -cst, {nv, nc} });
				}
			}
			vis[nc] = true;
		}

		if (nod != -1)
			ans.push_back({{nod, nc}, -c});
	}

	return ans;
}

void Graph::generateGraph()
{
	srand(static_cast<unsigned>(time(NULL)));
	generateNodes();
	generateEdges();
}

std::vector<olc::vi2d> Graph::getNodesPositions(const std::vector<Node>& nodes) const
{
	std::vector<olc::vi2d> positions;

	for (const auto& node : nodes) {
		positions.push_back(node.pos);
	}

	return positions;
}

Graph::Graph()
{
}

std::vector<Node> Graph::getNodes() const
{
	return this->nodes;
}

std::vector<Edge> Graph::getEdges() const
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