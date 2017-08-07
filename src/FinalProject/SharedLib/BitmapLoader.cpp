#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "BitmapLoader.h"


BitmapData LoadBitmap(const char* filepath)
{
    std::ifstream stream (filepath, std::ios::binary);
    
    if (!stream.is_open())
    {
        printf("Failed to open file %s\n", filepath);
        exit(EXIT_FAILURE);
    }

    char header[14];
    stream.read(header, 14);

    if (header[0] != 'B' || header[1] != 'M')
    {
        printf("Header is incorrect\n", filepath);
        exit(EXIT_FAILURE);
    }

    int pixelDataLocation = Pack(header[13], header[12], header[11], header[10]);
    stream.read(header, 12);
    BitmapData result;
    result.Width = Pack(header[7], header[6], header[5], header[4]);
    result.Height = Pack(header[11], header[10], header[9], header[8]);
    
    stream.read(header, 8);
    result.BPP = Pack(0, 0, header[3], header[2]);

    stream.seekg(pixelDataLocation);
    result.Bytes = new char[result.Width * result.Height * result.BPP / 8];

    char* pos = result.Bytes;    
    int amt = result.Width * result.BPP / 8;
    char* buffer = new char[result.BPP / 8];

    for (int y = 0; y < result.Height; y++)
    {               
        //stream.read(buffer, amt);

        for (int i = 0; i < amt; i += result.BPP / 8)
        {
            stream.read(buffer, result.BPP / 8);
            for (int j = result.BPP / 8; j > 0; j--)
            {
                *pos = buffer[j - 1];
                pos++;
            }
        }

        //pos += amt;        

        // Skip over any padding
        if (amt % 4 != 0)
        {
            stream.read(header, amt % 4);
        }
    }    

    delete[] buffer;

    return result;
}

GLuint MakeGLTexture(BitmapData data, float anisotropy)
{
    GLuint result;
    glGenTextures(1, &result);

    glBindTexture(GL_TEXTURE_2D, result);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, data.Width, data.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data.Bytes);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    
    if (anisotropy > 1)
    {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, anisotropy);
    }
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    return result;
}