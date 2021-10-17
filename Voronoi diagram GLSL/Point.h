#pragma once

#include <raylib.h>

class Point
{
private:
	Vector2 position;
	Vector2 velocity;
	float timer = 0.f;
	float maxTimer;
	float speed;
	bool turn = false;
public:
	// constructor
	Point(Vector2 position, Vector2 velocity, float maxtimer, float speed);

	// per frame update method
	void update(const float delta);

	// getters
	Vector2 getPosition() { return position; }
	
	// setters
	void setPosition(Vector2 to) { position = to; }
};

