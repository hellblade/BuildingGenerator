#pragma once

#include <GL/glew.h>

typedef struct PositionTextureVertex
{
    float X;
    float Y;
    float Z;
    float TX;
    float TY;
} PositionTextureVertex;

class Terrain
{

public:
    void Create(int w, int l, int dx, int dy);
    void Draw();

private:
    GLuint m_vertexBuffer, m_indexBuffer;
    int indCount;
};