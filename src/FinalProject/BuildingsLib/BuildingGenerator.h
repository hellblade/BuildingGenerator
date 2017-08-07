#pragma once

#include <stdlib.h> 
#include <Buffers.h>
#include <GL/glut.h>
#include <Random.h>
#include "Building.h"
#include "BuildingTextureInfo.h"

typedef struct BuildingInfo
{
    float x;
    float y;
    float width;    
    float height;
    float depth;
    
    float maxRadius;
    float minRadius;    
    int numSubShapes;
    bool allowHoles;
    int cutOuts[2];
    float cutOutSize[2];    
    bool allowCutouts;

    int texture;
    bool allowBetterShapes;
    
    BuildingInfo()
    {
    }
    
} BuildingInfo;

namespace BuildingGenerator
{
   /* namespace Impl
    {
        BuildingVertex* GenerateNGon(const BuildingInfo &info, int numVerts);
        BuildingVertex* GenerateFloorPlan(const BuildingInfo &info, int numVerts);
        void GenerateBuffers(const BuildingInfo &info, BuildingVertex* floorPlan, int numVerts, GLuint &vertexBuffer, GLuint &indexBuffer, int &numIndices);
    }*/

    Building* GenerateBuilding(const BuildingInfo &info);
    BuildingInfo CreateInfo(float x, float y, float width, float depth, float height, bool allowBetterShapes);
   // void FreeBuildingInfo(BuildingInfo &info);
}