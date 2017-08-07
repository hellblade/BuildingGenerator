#pragma once

#include <GL/glew.h>

typedef struct BitmapData
{
    int Width;
    int Height;
    int BPP;
    char* Bytes;
} BitmapData;

BitmapData LoadBitmap(const char* filepath);

GLuint MakeGLTexture(BitmapData data, float anisotropy);

inline int Pack(char b1, char b2, char b3, char b4)
{
    return (b1 << 24) | (b2 << 16) | (b3 << 8) | b4;
}