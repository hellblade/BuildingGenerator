#pragma once

class QuadTree;

#include <GL/glew.h>
#include <GL/glut.h>
#include <vector>
#include <MathHelper.h>
#include "BuildingTextureInfo.h"
#include "QuadTree.h"

typedef struct BuildingVertex
{
    Vector3 position;
    Vector3 normal;
    Vector3 colour;    
    Vector3 textureCoords;
    Vector2 topTextureCoords;
} BuildingVertex;

class Building
{   
private:  
  GLuint m_vertexBuffer, m_indexBuffer;
  int indCount, textureIndex;
  
public:
    Building(GLuint, GLuint, int, int, std::vector<BuildingVertex>, std::vector<unsigned int>, Rectangle3D);
  ~Building();
  void Draw();

  std::vector<BuildingVertex> vertices;
  std::vector<unsigned int> indices;
  Rectangle3D bounds;

  friend class QuadTree;

};