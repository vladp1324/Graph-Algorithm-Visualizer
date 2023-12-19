#pragma once
#include <string>
#define RADIUS 9

class Node {
public:
	Node(){}
	Node(int id, int32_t x, int32_t y)
	{ 
		this->id = id;
		this->x = x;
		this->y = y; 
	}
	int getId() { return this->id; }
	int32_t getX() { return this->x; }
	int32_t getY() { return this->y; }
private:
	int id;
	int32_t x;
	int32_t y;

};