#pragma once
#define QUAD_TREE_CAPACITY 10

class Building;
class Frustum;

#include <stdio.h>
#include <vector>
#include <stack>
#include <GL/glew.h>
#include <GL/glut.h>
#include <MathHelper.h>
#include "Building.h"
#include "Frustum.h"

class QuadTree
{
private:
    Building *buildings[QUAD_TREE_CAPACITY];
    int count;
    QuadTree *topLeft, *topRight, *bottomLeft, *bottomRight;
    Rectangle3D bounds;

    void GetBuildings(Frustum *frustum, std::vector<Building*> &buildings1, std::vector<Building*> &buildings2);

public:
    QuadTree(Rectangle3D bounds);

    bool Add(Building *building);

    void DrawAll(Frustum *frustum);

};