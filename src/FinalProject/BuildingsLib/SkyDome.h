#pragma once

#include <GL/glew.h>
#include <GL/glut.h>
#include <MathHelper.h>

struct SkyDomeVertex
{
    Vector3 position;    
    Vector2 textureCoords;
};

class SkyDome
{
private:
    GLuint m_vertexBuffer, m_indexBuffer;
    GLuint randTexture;
    int indCount;

public:
    SkyDome();

    void Render();

};