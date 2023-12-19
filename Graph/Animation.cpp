#include "Animation.h"
#include "olcPixelGameEngine.h"

Animation::Animation(const std::vector<edge_for_anim>& animFrames, AnimationType t)
{
	this->frames = animFrames;
	this->type = t;
}

void Animation::update(float deltaTime)
{
	if (!isPlaying) {
		return;
	}

	currentTime += deltaTime;

	if (currentTime > animationCooldown) {
		currentFrame++;
		currentTime = 0;
	}

	if (currentFrame >= frames.size()) {
		currentFrame = 0;
		isPlaying = false;
	}
}

void Animation::render()
{
	for (int i = 0; i <= currentFrame; i++) {
		Edge edge = frames[i].e;
		Node node1 = edge.n1;
		Node node2 = edge.n2;
		//DrawLine(node1.x, node1.y, node2.x, node2.y, olc::RED);
	}
}
