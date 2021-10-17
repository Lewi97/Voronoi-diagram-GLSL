#include "VoronoiDiagram.h"
#include <iostream>

VoronoiDiagram::VoronoiDiagram(int screenWidth, int screenHeight)
{
    // set screensize
    screenSize = { (float)screenWidth, (float)screenHeight };

    // create canvas to draw shader to
    canvas = LoadRenderTexture(screenSize.x, screenSize.y);

    // load shader
    voronoi = LoadShader(0, "Shader/Voronoi.frag");
    
    // rectangle to with flipped y to go with shader
    shaderRect = { 0, 0, (float)canvas.texture.width, (float)-canvas.texture.height };
    
    // get uniform locations
    int u_resolution = GetShaderLocation(voronoi, "u_resolution"); // need this only once for now so we dont save it
    u_points = GetShaderLocation(voronoi, "u_points");
    u_colors = GetShaderLocation(voronoi, "u_colors");

    // create 32 random points
    for (int i = 0; i < numOfPoints; i++)
        points.emplace_back(createRandomPoint(radius, { screenSize.x, screenSize.y }));

    // create 32 random colors normalized to a 0...1 range
    Vector3* colorValues = new Vector3[numOfPoints];
    for (int i = 0; i < numOfPoints; i++)
    {
        colorValues[i].x = (float)GetRandomValue(1, 255) / 255.f;
        colorValues[i].y = (float)GetRandomValue(1, 255) / 255.f;
        colorValues[i].z = (float)GetRandomValue(1, 255) / 255.f;
    }

    // send resolution and colors to shader
    glUseProgram((GLuint)voronoi.id);
    float s_resolution[2] = { (float)canvas.texture.width, (float)canvas.texture.height };
    glUniform2fv((GLint)u_resolution, (GLsizei)1, (const GLfloat*)s_resolution);
    glUniform3fv((GLint)u_colors, (GLsizei)numOfPoints, (const GLfloat*)colorValues);
}

void VoronoiDiagram::run()
{
    // update all points
    updatePoints();

    // handle inputs
    handleInput();

    // send new data to the shader before drawing starts
    setShaderValues();

    // draw everything to the screen
    draw();
}

void VoronoiDiagram::updatePoints()
{
    const float delta = GetFrameTime();

    for (int i = 0; i < numOfPoints; i++)
    {
        Point& p = points[i];

        Vector2 vec = p.getPosition();
        // normalize position to a 0...1 range for shader
        vec.x /= screenSize.x;
        vec.y /= screenSize.y;
        tempValues[i] = vec;

        p.update(delta);
    }
}

void VoronoiDiagram::handleInput()
{
    const Vector2 mouse = GetMousePosition();

    // check if we clicked on a point
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        for (auto& p : points)
        {
            if (!pointInCircle(mouse, flipY(p.getPosition()), radius * 5)) continue;
            pointInMouse = &p;
            break;
        }

    // if mouse button held, set position of held point to mouse position
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
    {
        if (pointInMouse != nullptr)
            pointInMouse->setPosition(flipY(mouse));
    }
    else
        pointInMouse = nullptr;
}

void VoronoiDiagram::draw()
{
    BeginDrawing();

    ClearBackground(RAYWHITE);

    BeginShaderMode(voronoi);

    DrawTextureRec(canvas.texture, shaderRect, { 0.f,0.f }, WHITE);

    EndShaderMode();

    // draw every circle with a flipped y position to stay in line with opengl's coordinates
    for (auto& p : points)
        DrawCircleV(flipY(p.getPosition()), radius, BLACK);

    EndDrawing();
}

void VoronoiDiagram::setShaderValues()
{
    glUseProgram((GLuint)voronoi.id);
    glUniform2fv((GLint)u_points, (GLsizei)numOfPoints, (const GLfloat*)tempValues);
}

Point VoronoiDiagram::createRandomPoint(const float radius, const Vector2 screenSize)
{
    Vector2 pos = { GetRandomValue(radius, screenSize.x - radius), GetRandomValue(radius, screenSize.y - radius) };
    Vector2 vel = { (float)GetRandomValue(-10, 10) / 10, (float)GetRandomValue(-10, 10) / 10 };
    float speed = GetRandomValue(10, 50);
    float maxTimer = (float)GetRandomValue(50, 250) / 100;
    Point point(pos, vel, maxTimer, speed);
    return point;
}
