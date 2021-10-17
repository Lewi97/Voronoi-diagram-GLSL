#include "Point.h"

Point::Point(Vector2 position, Vector2 velocity, float maxtimer, float speed) :
	position(position), velocity(velocity), maxTimer(maxtimer), speed(speed)
{
}

// move point based on velocity, after timer expires change turn direction
void Point::update(const float delta)
{
	timer += delta;
	if (timer > maxTimer)
	{
		timer = 0.f;
		turn = !turn;
	}
	int toTurn = turn ? 1 : -1;
	position.x += velocity.x * speed * delta * toTurn;
	position.y += velocity.y * speed * delta * toTurn;
}

