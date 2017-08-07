#include "BuildingTextureInfo.h"
#include <G308_ImageLoader.h>

namespace BuildingTextureInfo
{

    GLuint *buildingTextures = NULL;
    GLuint *buildingNormalMaps = NULL;
    int buildingTexureCount = 3;
    int buildingTextureSideCount = 2;
    int buildingTextureRoofIndex = 2;

    GLuint MakeTexture(TextureInfo info)
    {
        GLuint result;
        glGenTextures(1, &result);
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

        glBindTexture(GL_TEXTURE_2D, result);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, info.width, info.height, 0, GL_RGB, GL_UNSIGNED_BYTE, info.textureData);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        glGenerateMipmap(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, 0);

        free(info.textureData);

        return result;
    }

    void Load()
    {
        buildingTexureCount = 3;
        buildingTextureSideCount = 2;
        buildingTextureRoofIndex = 2;

        buildingTextures = new GLuint[buildingTexureCount];
        buildingNormalMaps = new GLuint[buildingTexureCount];

        TextureInfo info;
        loadTextureFromJPEG("textures/buildings/b1.jpg", &info);
        buildingTextures[0] = MakeTexture(info);

        loadTextureFromJPEG("textures/buildings/b1normal.jpg", &info);
        buildingNormalMaps[0] = MakeTexture(info);

        loadTextureFromJPEG("textures/buildings/b2.jpg", &info);
        buildingTextures[1] = MakeTexture(info);

        loadTextureFromJPEG("textures/buildings/b2normal.jpg", &info);
        buildingNormalMaps[1] = MakeTexture(info);

        loadTextureFromJPEG("textures/buildings/roof.jpg", &info);
        buildingTextures[2] = MakeTexture(info);

        loadTextureFromJPEG("textures/buildings/roofNormal.jpg", &info);
        buildingNormalMaps[2] = MakeTexture(info);
    }

    void Unload()
    {
        for (int i = 0; i < buildingTexureCount; i++)
        {
            glDeleteTextures(1, buildingTextures + i);
        }

        delete buildingTextures;
        delete buildingNormalMaps;
    }

}