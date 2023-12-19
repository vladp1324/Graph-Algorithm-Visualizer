#pragma once
#include <vector>
#include "GraphRepresentation.h"

enum class AnimationType
{
	DFS,
	BFS,
	DIJKSTRA
};

class Animation {
public:
	Animation(const std::vector<edge_for_anim>& animFrames, AnimationType t);
	void update(float deltaTime);
	void render();

private:
	std::vector<edge_for_anim> frames;
	AnimationType type;
	float animationCooldown = 1.0f;
	float currentTime = 0.0f;
	int currentFrame = 0;
	bool isPlaying;

};