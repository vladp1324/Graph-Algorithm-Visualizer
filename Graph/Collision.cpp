#include "Collision.h"

bool checkCollisionCircles(olc::vd2d c1, olc::vd2d c2)
{
    int32_t distX = c1.x - c2.x;
    int32_t distY = c1.y - c2.y;
    int32_t distance = sqrt((distX * distX) + (distY * distY));

    return distance <= RADIUS * 6;
}

bool checkCollisionLineCircle(olc::vd2d P1, olc::vd2d P2, olc::vd2d C)
{
	olc::vd2d D = { P2.x - P1.x, P2.y - P1.y };
	olc::vd2d F = { P1.x - C.x, P1.y - C.y };

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

bool checkCollisionPointRect(olc::vd2d p, olc::vd2d rect, int32_t width, int32_t height)
{
	return rect.x <= p.x && p.x <= rect.x + width &&
		rect.y <= p.y && p.y <= rect.y + height;
}

bool checkCollisionPointCircle(olc::vd2d p, olc::vd2d c)
{
	int32_t distX = p.x - c.x;
	int32_t distY = p.y - c.y;
	int32_t distance = sqrt((distX * distX) + (distY * distY));

	return distance <= RADIUS;
}
