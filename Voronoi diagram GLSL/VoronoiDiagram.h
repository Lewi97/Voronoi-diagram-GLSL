#pragma once

#include <vector>

#include <external/glad.h>
#include <raylib.h>

#include "Point.h"

class VoronoiDiagram
{
private:
	// containers
	std::vector<Point> points;
	Vector2* tempValues = new Vector2[numOfPoints];
	Point* pointInMouse = nullptr;

	// uniforms
	int u_points;
	int u_colors;

	// constants
	const float radius = 2.5f;
	const int numOfPoints = 32;

	// window information
	Vector2 screenSize;
	
	// shader related
	RenderTexture2D canvas;
	Rectangle shaderRect;
	Shader voronoi;

	// private methods
	void updatePoints();
	void handleInput();
	void draw();
	void setShaderValues();
public:
	// constructor
	VoronoiDiagram(int screenWidth, int screenHeight);

	// per frame update loop
	void run();

	// tools
	Point createRandomPoint(const float radius, const Vector2 screenSize);
	bool pointInCircle(const Vector2& p, const Vector2& c, const float r)
	{
		return (p.x - c.x) * (p.x - c.x) + (p.y - c.y) * (p.y - c.y) < r * r;
	}

	inline Vector2 flipY(const Vector2& toFlip)
	{
		return { toFlip.x, (float)screenSize.y - toFlip.y };
	}
};

