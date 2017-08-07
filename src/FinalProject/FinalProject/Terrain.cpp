#include <stdlib.h>
#include <GL/glew.h>
#include <Buffers.h>
#include "Terrain.h"

void Terrain::Create(int width, int length, int dx, int dz)
{
    int numVertsX = width / dx + 1;
    int numVertsZ = length / dz + 1;

    PositionTextureVertex *verts = new PositionTextureVertex[numVertsX * numVertsZ];
    
    for (int x = 0; x <= width; x += dx)
    {
        for (int z = 0; z <= length; z += dz)
        {
            PositionTextureVertex *currentVert = &verts[x /dx + z / dz * numVertsX];

            currentVert->X = x - width / 2;
            currentVert->Y = 0;
            currentVert->Z = z - length / 2;
            currentVert->TX = (float)x / width * dx;
            currentVert->TY = (float)z / length * dz;
        }
    }

    unsigned int *indices = new unsigned int[(numVertsX - 1) * (numVertsZ - 1) * 6];

    int current = 0;
    for (int x = 0; x < numVertsX - 1; x++)
    {
        for (int z = 0; z < numVertsZ - 1; z++)
        {
            unsigned int lowerLeft = x + z * numVertsX;
            unsigned int lowerRight = (x + 1) + z * numVertsX;
            unsigned int topLeft = x + (z + 1) * numVertsX;
            unsigned int topRight = (x + 1) + (z + 1) * numVertsX;
            
            indices[current++] = lowerLeft;
            indices[current++] = lowerRight;
            indices[current++] = topLeft;
            
            indices[current++] = lowerRight;
            indices[current++] = topLeft;
            indices[current++] = topRight;            
        }
    }

    indCount = current;

   /* glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, numVertsX * numVertsZ * sizeof(PositionTextureVertex), verts, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/
    m_vertexBuffer = Buffers::GenVertexBuffer(numVertsX * numVertsZ * sizeof(PositionTextureVertex), verts);
    m_indexBuffer = Buffers::GenIndexBuffer(indCount * sizeof(unsigned int), indices);

    delete[] verts;
    delete[] indices;
}

void Terrain::Draw()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(PositionTextureVertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(PositionTextureVertex), (unsigned char*)NULL + (3 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, 0);    
}