#pragma once

#include <GL/glew.h>
#include <GL/glut.h>

namespace Buffers
{
    GLuint GenVertexBuffer(int sizeInBytes, void* data);
    GLuint GenIndexBuffer(int sizeInBytes, void* data);
}
