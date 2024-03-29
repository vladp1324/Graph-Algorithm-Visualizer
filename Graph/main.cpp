#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "Graph.h"
#include "Collision.h"
#include <stdio.h> 
#include <stdlib.h>

class GraphVisualizer : public olc::PixelGameEngine {

private:
	//variables
	
	//graph
	Graph G;
	std::vector <Node> nodes;
	std::vector <Edge> edges;
	
	//button type
	enum ButtonType {
		GENERATE,
		COSTS,
		BFS,
		DFS,
		DIJKSTRA,
		PRIM,
		AUTO_ANIMATION,
		PREV,
		NEXT
	};

	//buttons
	struct Button {
		olc::vi2d pos;
		int32_t width;
		int32_t height;

		std::string text;
		olc::vi2d posText;
		
		ButtonType type;
	};
	std::vector<Button> buttons;

	//buttonsNodes
	struct ButtonNode {
		int idNumber;
		std::string idCh;
		olc::vi2d posText;
	};
	std::vector < ButtonNode > buttonsNodes;

	int sourceNode;
	bool animationBfsOn;
	bool animationDfsOn;
	bool animationDijkstraOn;
	bool animationPrimOn;
	bool showCosts;
	bool autoAnimation;

	bool isButtonSelected;
	
	int index = -1;
	std::vector<edge_for_anim> steps;

	void constructGraph() {
		//auto animation: on
		autoAnimation = true;

		//showCosts = false;
		sourceNode = -1;
		buttonsNodes.clear();

		G.generateGraph();
		nodes = G.getNodes();
		edges = G.getEdges();

		for (const auto& node : nodes) {
			char letterChar = node.id + 'A';
			std::string letter = std::string(1, letterChar);
			ButtonNode btnNode = { node.id, letter, olc::vi2d{node.pos.x - 3, node.pos.y - 3} };
			buttonsNodes.push_back(btnNode);
		}
	}

	void UpdateBFSAnimation(const float& deltaTime) {
		if (animationBfsOn == false)
			return;

		//draw edges
		for (int i = 0; i <= index; i++) {
			Edge edge = steps[i].e;
			Node node1 = nodes[edge.idn1];
			Node node2 = nodes[edge.idn2];
			int cost = edge.cost;
			AnimType type = steps[i].type;
			
			if (type == VISITED) {
				DrawLine(node1.pos, node2.pos, olc::RED);
				DrawCircle(node2.pos, RADIUS + 1, olc::RED);

				//draw in table
				char letterChar = node1.id + 'A';
				std::string letter = std::string(1, letterChar);
				
				if (sourceNode == node2.id)
					letter = "-";

				DrawString(355 + node2.id * 24, 40, letter);
				
				int z = 0;
				if (cost > 9)
					z = 5;

				DrawString(355 + node2.id * 24 - z, 60, std::to_string(cost));
			}
			else {
				DrawLine(node1.pos, node2.pos, olc::YELLOW);
				DrawCircle(node2.pos, RADIUS + 1, olc::YELLOW);
			}
		}

		static float ed = 0;

		if (autoAnimation) {
			if (ed > 1.0f) {
				if (index + 1 < steps.size())
					index++;
				ed = 0;
			}
			ed += deltaTime;
		}
	}

	void UpdateDFSAnimation(const float& deltaTime) {
		if (animationDfsOn == false)
			return;

		char letterChar;
		std::string letter;

		int indexYellow = 0;
		int indexRed = 0;

		//draw edges
		for (int i = 0; i <= index; i++) {
			Edge edge = steps[i].e;
			Node node1 = nodes[edge.idn1];
			Node node2 = nodes[edge.idn2];
			int cost = edge.cost;
			AnimType type = steps[i].type;

			if (type == VISITED) {
				DrawLine(node1.pos, node2.pos, olc::RED);
				//DrawCircle(nodes[steps[i].e.idn2].pos, RADIUS + 1, olc::RED);
				DrawCircle(node1.pos, RADIUS + 1, olc::RED);

				//draw in table
				letterChar = node1.id + 'A';
				letter = std::string(1, letterChar);
				DrawString(310 + indexRed * 24, 100, letter);

				int z = 0;
				if (cost > 9)
					z = 5;

				DrawString(310 + indexRed * 24 - z, 120, std::to_string(cost));
				indexRed++;
			}
			else {
				DrawLine(node1.pos, node2.pos, olc::YELLOW);
				DrawCircle(node2.pos, RADIUS + 1, olc::YELLOW);
			
				//draw in table
				letterChar = node2.id + 'A';
				letter = std::string(1, letterChar);
				DrawString(310 + indexYellow * 24, 40, letter);
				
				int z = 0;
				if (cost > 9)
					z = 5;

				DrawString(310 + indexYellow * 24 - z, 60, std::to_string(cost));
				indexYellow++;
			}
		}

		static float ed = 0;

		if (autoAnimation) {
			if (ed > 1.0f) {
				if (index + 1 < steps.size())
					index++;
				ed = 0;
			}
			ed += deltaTime;
		}
	}

	void UpdateDijkstraAnimation(const float& deltaTime) {
		if (animationDijkstraOn == false)
			return;

		//draw edges
		for (int i = 0; i <= index; i++) {
			Edge edge = steps[i].e;
			Node node1 = nodes[edge.idn1];
			Node node2 = nodes[edge.idn2];
			int cost = edge.cost;
			AnimType type = steps[i].type;

			if (type == VISITED) {
				DrawLine(node1.pos, node2.pos, olc::RED);
				DrawCircle(node2.pos, RADIUS + 1, olc::RED);

				//draw in table
				char letterChar = node1.id + 'A';
				std::string letter = std::string(1, letterChar);

				if (sourceNode == node2.id)
					letter = "-";
				DrawString(355 + node2.id * 24, 40, letter);
			
				int z = 0;
				if (cost > 9)
					z = 5;

				DrawString(355 + node2.id * 24 - z, 60, std::to_string(cost));
			}
			else{
				DrawLine(node1.pos, node2.pos, olc::YELLOW);
				DrawCircle(node2.pos, RADIUS + 1, olc::YELLOW);
			}
		}

		static float ed = 0;

		if (autoAnimation) {
			if (ed > 1.0f) {
				if (index + 1 < steps.size())
					index++;
				ed = 0;
			}
			ed += deltaTime;
		}
	}

	void UpdatePrimAnimation(const float& deltaTime)
	{
		if (animationPrimOn == false)
			return;

		int totalCost = 0;

		//draw edges
		for (int i = 0; i <= index; i++) {
			Edge edge = steps[i].e;
			Node node1 = nodes[edge.idn1];
			Node node2 = nodes[edge.idn2];
			int cost = edge.cost;
			AnimType type = steps[i].type;

			if (type == VISITED) {
				DrawLine(node1.pos, node2.pos, olc::RED);
				DrawCircle(node2.pos, RADIUS + 1, olc::RED);
				//draw in table
				char letterChar = node1.id + 'A';
				std::string letter = std::string(1, letterChar);

				if (sourceNode == node2.id)
					letter = "-";

				totalCost += cost;
				DrawString(355 + node2.id * 24, 40, letter);
			}
			else{
				DrawLine(node1.pos, node2.pos, olc::YELLOW);
				DrawCircle(node2.pos, RADIUS + 1, olc::YELLOW);
			}

		}

		DrawString(400, 60, std::to_string(totalCost));

		static float ed = 0;

		if (autoAnimation) {
			if (ed > 1.0f) {
				if (index + 1 < steps.size())
					index++;
				ed = 0;
			}
			ed += deltaTime;
		}
	}

	void drawScreenBorder() {
		DrawRect(10, 10, ScreenWidth() - 20, ScreenHeight() - 20);
		DrawLine(300, 10, 300, ScreenHeight() - 10);
		DrawLine(10, 250, 300, 250);
	}

	void drawEdges() {
		for (const auto& edge : edges) {
			DrawLine(nodes[edge.idn1].pos, nodes[edge.idn2].pos, olc::GREEN);
		}
	}

	void drawCosts() {
		if (showCosts) {
			for (const auto& edge : edges) {
				float posCostX = (nodes[edge.idn1].pos.x + nodes[edge.idn2].pos.x) / 2 - 9;
				float posCostY = (nodes[edge.idn1].pos.y + nodes[edge.idn2].pos.y) / 2 - 5;

				std::string cost = std::to_string(edge.cost);

				FillRect(posCostX, posCostY, 18, 10, olc::CYAN);
				DrawRect(posCostX - 1, posCostY - 1, 19, 11, olc::WHITE);
				
				float posTextX = posCostX + 1;
				float posTextY = posCostY + 1;
				
				if (edge.cost < 10)
					posTextX += 4;

				DrawString(posTextX, posTextY, cost, olc::RED);
			}
		}
	}

	void drawNodes() {
		for (const auto& node_button : buttonsNodes) {
			if (node_button.idNumber == sourceNode)
				continue;

			FillCircle(nodes[node_button.idNumber].pos, RADIUS);
			DrawString(nodes[node_button.idNumber].pos.x - 3, nodes[node_button.idNumber].pos.y - 3, node_button.idCh, olc::MAGENTA);
		}

		if (sourceNode != -1) {//draw selected node
			FillCircle(nodes[sourceNode].pos, RADIUS);
			DrawString(nodes[sourceNode].pos.x - 3, nodes[sourceNode].pos.y - 3, buttonsNodes[sourceNode].idCh, olc::MAGENTA);
			DrawCircle(nodes[sourceNode].pos, RADIUS + 1, olc::RED);
		}
	}

	void drawGraph(const float& deltaTime) {
		
		drawEdges();
		
		UpdateBFSAnimation(deltaTime);

		UpdateDFSAnimation(deltaTime);

		UpdateDijkstraAnimation(deltaTime);

		UpdatePrimAnimation(deltaTime);
		
		drawCosts();
		
		drawNodes();
	}

	void initButtons() {
		Button buttonGenerateGraph = { {30, 260}, 75, 20, "Generate", {36, 267}, GENERATE};
		Button buttonShowCosts = { {230, 260}, 50, 20, "Costs", {236, 267}, COSTS};
		
		Button buttonStartBfs = { {405, 230}, 35, 20, "BFS", {411, 237}, BFS};
		Button buttonStartDfs = { {405, 260}, 35, 20, "DFS", {411, 267}, DFS};
		Button buttonStartDijkstra = { {320, 260}, 75, 20, "DIJKSTRA", {326, 267}, DIJKSTRA};
		Button buttonStartPrim = { {335, 230}, 44, 20, "PRIM", {341, 237}, PRIM};

		Button buttonAutoAnimation = { {500, 230}, 74, 20, "", {506, 237}, AUTO_ANIMATION};
		Button buttonPrev = { {500, 260}, 20, 20, "<", {506, 267}, PREV };
		Button buttonNext = { {554, 260}, 20, 20, ">", {560, 267}, NEXT };



		buttons.push_back(buttonGenerateGraph);
		buttons.push_back(buttonShowCosts);

		buttons.push_back(buttonStartBfs);
		buttons.push_back(buttonStartDfs);
		buttons.push_back(buttonStartDijkstra);
		buttons.push_back(buttonStartPrim);

		buttons.push_back(buttonAutoAnimation);
		buttons.push_back(buttonPrev);
		buttons.push_back(buttonNext);
	}

	void drawButtons() {
		for (const auto& btn : buttons) {
			FillRect(btn.pos.x, btn.pos.y, btn.width, btn.height, olc::DARK_GREY);
			DrawString(btn.posText.x, btn.posText.y, btn.text, olc::WHITE);
		}
	}

	void resetStateAnimation() {
		index = -1;

		//reset state bfs
		animationBfsOn = false;
		
		//reset state dfs
		animationDfsOn = false;

		//reset state dijkstra
		animationDijkstraOn = false;

		//reset state prim
		animationPrimOn = false;
	}

	void handleAutoAnimation() {
		for (auto& btn : buttons)
			if (btn.type == AUTO_ANIMATION) {

				if (autoAnimation)
					btn.text = "AUTO:ON";
				else
					btn.text = "AUTO:OFF";
			}
	}

	void checkButtonsPressed(const olc::vi2d& mousePos) {
		for (const auto& btn : buttons) {
			if (!isButtonSelected && checkCollisionPointRect(mousePos, btn.pos, btn.width, btn.height)) {
				//select button
				FillRect(btn.pos.x, btn.pos.y, btn.width, btn.height, olc::VERY_DARK_GREY);
				DrawString(btn.posText.x, btn.posText.y, btn.text, olc::GREY);

				//handle mouse pressed
				if (GetMouse(0).bPressed) {

					if (sourceNode == -1)
						sourceNode = 0;

					switch (btn.type) {
					case GENERATE: //button generate graph
						resetStateAnimation();
						constructGraph();
						break;
					
					case COSTS: //button show costs
						showCosts = 1 - showCosts;
						break;
					
					case BFS: //button start bfs
						resetStateAnimation();
						animationBfsOn = true;
						steps = G.BFS(sourceNode);
						break;
					
					case DFS: //button start dfs
						resetStateAnimation();
						animationDfsOn = true;
						steps = G.DFS(sourceNode);
						break;

					case DIJKSTRA: //button start dijkstra
						resetStateAnimation();
						animationDijkstraOn = true;
						steps = G.DIJKSTRA(sourceNode);
						break;

					case PRIM: //button start prim
						resetStateAnimation();
						animationPrimOn = true;
						steps = G.PRIM(sourceNode);
						break;

					case AUTO_ANIMATION: //button auto animation: on/off
						autoAnimation = 1 - autoAnimation;
						break;

					case PREV: //button previous animation state
						if(index - 1 >= -1)
							index--;
						break;

					case NEXT: //button next animation state
						if(index + 1 < steps.size())
							index++;
						break;

					default:
						break;
					}
				}
			}
		}
	}

	bool checkAnimationOn() {
		return animationBfsOn || animationDfsOn || animationDijkstraOn || animationPrimOn;
	}

	void checkButtonsNodesPressed(const olc::vi2d& mousePos) {
		for (const auto& node_button : buttonsNodes) {
			if (checkCollisionPointCircle(mousePos, nodes[node_button.idNumber].pos)) {
				if (!isButtonSelected) {
					FillCircle(nodes[node_button.idNumber].pos, RADIUS, olc::GREY);
					DrawString(nodes[node_button.idNumber].pos.x - 3, nodes[node_button.idNumber].pos.y - 3, node_button.idCh, olc::DARK_MAGENTA);
				}

				if (GetMouse(0).bPressed) {
					resetStateAnimation();
					//if(!checkAnimationOn())
					sourceNode = node_button.idNumber;
				}

				if (node_button.idNumber == sourceNode && GetMouse(0).bHeld) {
					isButtonSelected = true;
					olc::vi2d prevPos = nodes[sourceNode].pos;

					nodes[sourceNode].pos = { mousePos.x, mousePos.y };

					//right line
					if (checkCollisionLineCircle({ 300, 10 }, { 300, ScreenHeight() - 10 }, { nodes[sourceNode].pos }))
						nodes[sourceNode].pos = prevPos;

					//left line
					if (checkCollisionLineCircle({ 10, 10 }, { 10, ScreenHeight() - 10 }, { nodes[sourceNode].pos }))
						nodes[sourceNode].pos = prevPos;

					//top line
					if (checkCollisionLineCircle({ 10, 10 }, { 300, 10 }, { nodes[sourceNode].pos }))
						nodes[sourceNode].pos = prevPos;

					//bottom line
					if (checkCollisionLineCircle({10, 250}, { 300, 250 }, { nodes[sourceNode].pos }))
						nodes[sourceNode].pos = prevPos;
				}

				if (GetMouse(0).bReleased)
					isButtonSelected = false;
			}
		}
	}

	void handleKeys(){
		olc::vi2d p = GetMousePos();
		checkButtonsPressed(p);
		checkButtonsNodesPressed(p);
	}

	void drawTableBfs() {
		int xStart = 355;
		int yNode = 22;

		for (const auto& node_button : buttonsNodes) {
			DrawString(xStart + node_button.idNumber * 24, yNode, node_button.idCh);
		}

		DrawString(310, 40, "Prev:");
		DrawString(310, 60, "Dist:");
	}

	void drawTableDfs() {
		DrawString(310, 20, "Time to reach:");
		DrawString(310, 80, "Time to visit:");
	}

	void drawTableDijkstra() {
		int xStart = 355;
		int yNode = 22;

		for (const auto& node_button : buttonsNodes) {
			DrawString(xStart + node_button.idNumber * 24, yNode, node_button.idCh);
		}

		DrawString(310, 40, "Prev:");
		DrawString(310, 60, "Cost:");
	}

	void drawTablePrim() {
		int xStart = 355;
		int yNode = 22;

		for (const auto& node_button : buttonsNodes) {
			DrawString(xStart + node_button.idNumber * 24, yNode, node_button.idCh);
		}

		DrawString(310, 40, "Prev:");
		DrawString(310, 60, "Total cost:");
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

		if (animationPrimOn) {
			drawTablePrim();
		}
	}

public:
	GraphVisualizer() {
		sAppName = "GraphVisualizer";
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

		handleAutoAnimation();
		
		return true;
	}
};

int main() {
	GraphVisualizer demo;
	
	if (demo.Construct(600, 300, 2, 2))
		demo.Start();

	return 0;
}