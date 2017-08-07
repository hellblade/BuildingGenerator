#pragma once

#include <GL/glew.h>

namespace BuildingTextureInfo
{

    extern GLuint *buildingTextures;
    extern GLuint *buildingNormalMaps;
    extern int buildingTexureCount;
    extern int buildingTextureSideCount;
    extern int buildingTextureRoofIndex;

    void Load();

    void Unload();
}