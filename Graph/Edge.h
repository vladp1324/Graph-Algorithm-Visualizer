#pragma once
#include "Node.h"

class Edge {

public:
	Edge(Node n1, Node n2)
	{
		this->n1 = n1;
		this->n2 = n2;
		this->cost = rand() % 15 + 1;
	}
	
	Node getN1() { return n1; }
	Node getN2() { return n2; }
	int getCost() { return cost; }

private:
	Node n1;
	Node n2;
	int cost;
};