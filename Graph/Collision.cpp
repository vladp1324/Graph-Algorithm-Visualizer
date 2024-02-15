#include "Collision.h"

bool checkCollisionCircles(const olc::vi2d& c1, const olc::vi2d& c2, const int& nrCircles)
{
    int32_t distX = c1.x - c2.x;
    int32_t distY = c1.y - c2.y;
    int32_t distance = sqrt((distX * distX) + (distY * distY));

    return distance <= RADIUS * nrCircles;
}

bool checkCollisionLineCircle(const olc::vi2d& P1, const olc::vi2d& P2, const olc::vi2d& C)
{
	olc::vi2d D = { P2.x - P1.x, P2.y - P1.y };
	olc::vi2d F = { P1.x - C.x, P1.y - C.y };

	float a = D.x * D.x + D.y * D.y;
	float b = 2 * (F.x * D.x + F.y * D.y);
	float c = F.x * F.x + F.y * F.y - RADIUS * RADIUS;

	float discriminant = b * b - 4 * a * c;

	if (discriminant < 0) {
		// No collision
		return false;
	}

	// Check if the line segment intersects the circle
	float t1 = (-b + sqrt(discriminant)) / (2 * a);
	float t2 = (-b - sqrt(discriminant)) / (2 * a);

	if (t1 >= 0 && t1 <= 1 || t2 >= 0 && t2 <= 1) {
		// Intersection detected
		return true;
	}

	// No intersection
	return false;
}

bool checkCollisionPointRect(const olc::vi2d& p, const olc::vi2d& rect, const int32_t& width, const int32_t& height)
{
	return rect.x <= p.x && p.x <= rect.x + width &&
		rect.y <= p.y && p.y <= rect.y + height;
}

bool checkCollisionPointCircle(const olc::vi2d& p, const olc::vi2d& c)
{
	int32_t distX = p.x - c.x;
	int32_t distY = p.y - c.y;
	int32_t distance = sqrt((distX * distX) + (distY * distY));

	return distance <= RADIUS;
}

bool checkCollisionCircleVectorCircles(const olc::vi2d& c, const std::vector<olc::vi2d>& vc)
{
	for (auto const& circle : vc) {
		if (c.x == circle.x && c.y == circle.y)
			continue;

		if (checkCollisionCircles(c, circle, 2))
			return true;
	}

	return false;
}

bool simulateLineCollision(const olc::vi2d& p1, const olc::vi2d& p2, const std::vector<olc::vi2d>& vc)
{
	for (const auto& pos : vc) {
		if (p1 != olc::vi2d(pos.x, pos.y) && p2 != olc::vi2d(pos.x, pos.y)
			&& checkCollisionLineCircle(olc::vi2d(p1.x, p1.y), olc::vi2d(p2.x, p2.y), olc::vi2d(pos.x, pos.y))) {
			return true;
		}
	}

	return false;
}
