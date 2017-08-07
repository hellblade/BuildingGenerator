#include <stdlib.h>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Building.h"

Building::Building(GLuint vertexBuffer, GLuint indexBuffer, int indiciesCount, int tex, std::vector<BuildingVertex> verts, std::vector<unsigned int> inds, Rectangle3D rect)
{
    m_vertexBuffer = vertexBuffer;
    m_indexBuffer = indexBuffer;
    indCount = indiciesCount;       
    textureIndex = tex;
    vertices = verts;
    indices = inds;
    bounds = rect;
}

Building::~Building()
{
    if (m_vertexBuffer != 0)
    {
        glDeleteBuffers(1, &m_vertexBuffer);
        m_vertexBuffer = 0;
    }

    if (m_indexBuffer != 0)
    {
        glDeleteBuffers(1, &m_indexBuffer);
        m_indexBuffer = 0;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Building::Draw()
{                
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    // Bind side windows
    glBindTexture(GL_TEXTURE_2D, BuildingTextureInfo::buildingTextures[textureIndex]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, BuildingTextureInfo::buildingNormalMaps[textureIndex]);

    // Bind front windows
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, BuildingTextureInfo::buildingTextures[BuildingTextureInfo::buildingTextureRoofIndex]);
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, BuildingTextureInfo::buildingNormalMaps[BuildingTextureInfo::buildingTextureRoofIndex]);

    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(BuildingVertex), NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(BuildingVertex), (unsigned char*)NULL + (3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(BuildingVertex), (unsigned char*)NULL + (6 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, false, sizeof(BuildingVertex), (unsigned char*)NULL + (9 * sizeof(float)));

    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 2, GL_FLOAT, false, sizeof(BuildingVertex), (unsigned char*)NULL + (12 * sizeof(float)));

    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, 0);

    for (int i = 0; i < 5; i++)
    {
        glDisableVertexAttribArray(i);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
}