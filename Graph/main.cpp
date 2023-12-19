#include "olcPixelGameEngine.h"
#include "Graph.h"
#include "Collision.h"
#include <conio.h>
#include <stdio.h> 
#include <stdlib.h>

class Example : public olc::PixelGameEngine {
private:
	//variables
	int i = 0;
	//graph
	Graph G;
	std::vector <Node> nodes;
	std::vector <Edge> edges;

	//buttons
	struct Button {
		olc::vd2d pos;
		int32_t width;
		int32_t height;
		std::string text;
		olc::vd2d posText;
	};
	std::vector<Button> buttons;

	//buttonsNodes
	struct ButtonNode {
		olc::vd2d pos;
		std::string id;
		olc::vd2d posText;
	};
	std::vector < ButtonNode > buttonsNodes;

	int sourceNode;
	bool animationBfsOn;
	bool animationDfsOn;
	bool animationDijkstraOn;

	bool showCosts;

	void constructGraph() {
		showCosts = false;
		sourceNode = -1;
		buttonsNodes.clear();

		G.generateGraph();
		nodes = G.getNodes();
		edges = G.getEdges();

		for (int i = 0; i < nodes.size(); i++) {
			char letterChar = nodes[i].id + 'A';
			std::string letter = std::string(1, letterChar);
			ButtonNode btnNode = { olc::vd2d{(double)nodes[i].x, (double)nodes[i].y}, letter, olc::vd2d{(double)nodes[i].x - 3, (double)nodes[i].y - 3} };
			buttonsNodes.push_back(btnNode);
		}
	}

	int index = 0;
	std::vector<edge_for_anim> steps;

	void UpdateBFSAnimation(float deltaTime) {
		if (animationBfsOn == false)
			return;

		if (sourceNode == -1)
			sourceNode = 0;

		steps = G.BFS(sourceNode);

		//draw in table source
		DrawString(355 + steps[0].e.n1.id * 24, 40, "-");
		DrawString(355 + steps[0].e.n1.id * 24, 60, std::to_string(0));

		//draw edges
		for (int i = 0; i <= index; i++) {
			Edge edge = steps[i].e;
			Node node1 = edge.n1;
			Node node2 = edge.n2;
			
			DrawLine(node1.x, node1.y, node2.x, node2.y, olc::RED);
			
			//draw in table
			char letterChar = steps[i].e.n1.id + 'A';
			std::string letter = std::string(1, letterChar);
			DrawString(355 + steps[i].e.n2.id * 24, 40, letter);
			DrawString(355 + steps[i].e.n2.id * 24, 60, std::to_string(steps[i].dist));
			DrawCircle(steps[i].e.n2.x, steps[i].e.n2.y, RADIUS + 1, olc::RED);
		}


		static float ed = 0;

		if (ed > 1.0f) {
			if(index + 1 < steps.size())
				index++;
			ed = 0;
		}

		ed += deltaTime;
	}

	void UpdateDFSAnimation(float deltaTime) {
		if (animationDfsOn == false)
			return;

		if (sourceNode == -1)
			sourceNode = 0;

		steps = G.DFS(sourceNode);

		//draw source node
		char letterChar = steps[0].e.n1.id + 'A';
		std::string letter = std::string(1, letterChar);
		DrawString(365 + i * 24, 40, letter);
		DrawString(365 + i * 24, 80, std::to_string(0));

		//draw edges
		for (int i = 0; i <= index; i++) {
			Edge edge = steps[i].e;
			Node node1 = edge.n1;
			Node node2 = edge.n2;

			DrawLine(node1.x, node1.y, node2.x, node2.y, olc::RED);

			//draw in table
			int yVal = 0;
			int xVal = 0;
			if (365 + i * 24 + 24 >= 590) {
				yVal += 20;
				xVal += 240;
			}

			char letterChar = steps[i].e.n2.id + 'A';
			std::string letter = std::string(1, letterChar);
			DrawString(365 + i * 24 + 24 - xVal, 40 + yVal, letter);
			DrawString(365 + i * 24 + 24 - xVal - 0.015*xVal, 80 + yVal, std::to_string(steps[i].dist));
			DrawCircle(steps[i].e.n2.x, steps[i].e.n2.y, RADIUS + 1, olc::RED);
		}

		static float ed = 0;

		if (ed > 1.0f) {
			if (index + 1 < steps.size())
				index++;
			ed = 0;
		}
		ed += deltaTime;

	}

	void UpdateDijkstraAnimation(float deltaTime) {
		if (animationDijkstraOn == false)
			return;

		if (sourceNode == -1)
			sourceNode = 0;

		steps = G.DIJKSTRA(sourceNode);

		//draw in table source
		DrawString(355 + steps[0].e.n1.id * 24, 40, "-");
		DrawString(355 + steps[0].e.n1.id * 24, 60, std::to_string(0));

		//draw edges
		for (int i = 0; i <= index; i++) {
			Edge edge = steps[i].e;
			Node node1 = edge.n1;
			Node node2 = edge.n2;

			DrawLine(node1.x, node1.y, node2.x, node2.y, olc::RED);

			//draw in table
			char letterChar = steps[i].e.n1.id + 'A';
			std::string letter = std::string(1, letterChar);

			DrawString(355 + steps[i].e.n2.id * 24, 40, letter);
			DrawString(355 + steps[i].e.n2.id * 24, 60, std::to_string(steps[i].dist));
			DrawCircle(steps[i].e.n2.x, steps[i].e.n2.y, RADIUS + 1, olc::RED);
		}

		static float ed = 0;

		if (ed > 1.0f) {
			if (index + 1 < steps.size())
				index++;
			ed = 0;
		}

		ed += deltaTime;
	}

	void drawScreenBorder() {
		DrawRect(10, 10, ScreenWidth() - 20, ScreenHeight() - 20);
		DrawLine(300, 10, 300, ScreenHeight() - 10);
	}

	void drawEdges() {
		for (int i = 0; i < edges.size(); i++) {
			DrawLine(edges[i].n1.x, edges[i].n1.y, edges[i].n2.x, edges[i].n2.y, olc::YELLOW);
		}
	}

	void drawCosts() {
		if (showCosts) {
			for (int i = 0; i < edges.size(); i++) {
				float posCostX = (edges[i].n1.x + edges[i].n2.x) / 2; posCostX -= 0.02f * posCostX;
				float posCostY = (edges[i].n1.y + edges[i].n2.y) / 2; posCostY -= 0.01f * posCostY;
				std::string cost = std::to_string(edges[i].cost);

				FillRect(posCostX, posCostY, 18, 10, olc::CYAN);
				DrawRect(posCostX - 1, posCostY - 1, 19, 11, olc::WHITE);
				DrawString(posCostX + 1, posCostY + 1, cost, olc::RED);
			}
		}
	}

	void drawNodes() {
		for (int i = 0; i < buttonsNodes.size(); i++) {
			ButtonNode node = buttonsNodes[i];

			FillCircle(node.pos.x, node.pos.y, RADIUS);
			DrawString(node.posText.x, node.posText.y, node.id, olc::MAGENTA);
			
			if (i == sourceNode) {
				DrawCircle(node.pos.x, node.pos.y, RADIUS+1, olc::RED);
			}
		}
	}

	void drawGraph(float deltaTime) {
		
		drawEdges();
		
		UpdateBFSAnimation(deltaTime);

		UpdateDFSAnimation(deltaTime);

		UpdateDijkstraAnimation(deltaTime);
		
		drawCosts();
		
		drawNodes();
	}

	void initButtons() {
		Button buttonGenerateGraph = { {60, 260}, 75, 20, "Generate", {66, 267} };
		Button buttonShowCosts = { {200, 260}, 50, 20, "Costs", {206, 267} };
		Button buttonStartBfs = { {340, 230}, 35, 20, "BFS", {346, 237} };
		Button buttonStartDfs = { {410, 230}, 35, 20, "DFS", {416, 237} };
		Button buttonStartDijkstra = { {480, 230}, 75, 20, "DIJKSTRA", {486, 237} };

		buttons.push_back(buttonGenerateGraph);
		buttons.push_back(buttonShowCosts);
		buttons.push_back(buttonStartBfs);
		buttons.push_back(buttonStartDfs);
		buttons.push_back(buttonStartDijkstra);
	}

	void drawButtons() {
		for (auto btn : buttons) {
			FillRect(btn.pos.x, btn.pos.y, btn.width, btn.height, olc::DARK_GREY);
			DrawString(btn.posText.x, btn.posText.y, btn.text, olc::WHITE);
		}
	}

	void resetStateAnimation() {
		index = 0;

		//reset state bfs
		animationBfsOn = false;
		
		//reset state dfs
		animationDfsOn = false;

		//reset state dijkstra
		animationDijkstraOn = false;
	}

	void checkButtonsPressed(olc::vd2d mousePos) {

		for (int i = 0; i < buttons.size(); i++) {
			Button btn = buttons[i];

			if (checkCollisionPointRect(mousePos, btn.pos, btn.width, btn.height)) {
				//select button
				FillRect(btn.pos.x, btn.pos.y, btn.width, btn.height, olc::VERY_DARK_GREY);
				DrawString(btn.posText.x, btn.posText.y, btn.text, olc::GREY);

				//handle mouse pressed
				if (GetMouse(0).bPressed) {
					switch (i) {
					case 0: //button generate graph
						resetStateAnimation();
						constructGraph();
						break;
					
					case 1: //button show costs
						showCosts = (bool) (1 - (int)showCosts);
						break;
					
					case 2: //button start bfs
						resetStateAnimation();
						animationBfsOn = true;
						break;
					
					case 3: //button start dfs
						resetStateAnimation();
						animationDfsOn = true;
						break;
					case 4: //button start dijkstra
						resetStateAnimation();
						animationDijkstraOn = true;
						break;

					default:
						break;
					}
				}
			}
		}
		
	}
	
	void checkButtonsNodesPressed(olc::vd2d mousePos) {
		for (int i = 0; i < buttonsNodes.size(); i++) {
			ButtonNode btn = buttonsNodes[i];

			if (checkCollisionPointCircle(mousePos, btn.pos)) {
				FillCircle(btn.pos.x, btn.pos.y, RADIUS, olc::GREY);
				DrawString(btn.posText.x, btn.posText.y, btn.id, olc::DARK_MAGENTA);
				
				if (GetMouse(0).bPressed) {
					resetStateAnimation();
					sourceNode = i;
				}
			}
		}
	}

	void handleKeys(){
		olc::vd2d p = GetMousePos();
		checkButtonsPressed(p);
		checkButtonsNodesPressed(p);
	}

	void drawTableBfs() {
		int xStart = 355;
		int yNode = 22;

		for (int i = 0; i < buttonsNodes.size(); i++) {
			DrawString(xStart + i * 24, yNode, buttonsNodes[i].id);
		}

		DrawString(310, 40, "Prev:");
		DrawString(310, 60, "Dist:");
	}

	void drawTableDfs() {
		DrawString(310, 40, "Order:");
		DrawString(310, 80, "Time:");
	}

	void drawTableDijkstra() {
		int xStart = 355;
		int yNode = 22;

		for (int i = 0; i < buttonsNodes.size(); i++) {
			DrawString(xStart + i * 24, yNode, buttonsNodes[i].id);
		}

		DrawString(310, 40, "Prev:");
		DrawString(310, 60, "Dist:");
	}

	void drawTables() {


		if (animationBfsOn) {
			drawTableBfs();
		}

		if (animationDfsOn) {
			drawTableDfs();
		}

		if (animationDijkstraOn) {
			drawTableDijkstra();
		}
	}

public:
	Example() {
		sAppName = "Example";
		constructGraph();
	}

public:
	bool OnUserCreate() override {
		initButtons();
		
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override {
		Clear(olc::BLACK);
		
		drawScreenBorder();
		drawButtons();
		drawGraph(fElapsedTime);
		drawTables();

		handleKeys();
		
		return true;
	}
};

int main() {
	Example demo;
	
	if (demo.Construct(600, 300, 2, 2))
		demo.Start();

	return 0;
}

/*
TO DO LIST:
______________________
-clasa de animatie
-referinte si const
-rezolutie
*/