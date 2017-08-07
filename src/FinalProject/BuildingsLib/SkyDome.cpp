#include <stdio.h>
#include <iostream>
#include <fstream>
#include <Buffers.h>
#include <Random.h>
#include "SkyDome.h"

#define TEXTURE_SIZE 128

struct SkyDomeVertexImmedidate
{
    float x, y, z;
    float tu, tv;
    float nx, ny, nz;
};

SkyDome::SkyDome()
{
    char filename[] = "models/skydome.txt";

    using namespace std;
    ifstream fin;
    char input;
    int i;


    // Open the model file.
    fin.open(filename);

    // If it could not open the file then exit.
    if (fin.fail())
    {
        printf("Could not load skydome");
        exit(1);
    }

    // Read up to the value of vertex count.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }

    int vertexCount;

    // Read in the vertex count.
    fin >> vertexCount;

    // Set the number of indices to be the same as the vertex count.
    indCount = vertexCount;

    // Read up to the beginning of the data.
    fin.get(input);
    while (input != ':')
    {
        fin.get(input);
    }
    fin.get(input);
    fin.get(input);

    SkyDomeVertexImmedidate temp;
    SkyDomeVertex *vertices = new SkyDomeVertex[vertexCount];
    unsigned int * indices = new unsigned int[indCount];

    // Read in the vertex data.
    for (i = 0; i< vertexCount; i++)
    {
        fin >> temp.x >> temp.y >> temp.z;
        fin >> temp.tu >> temp.tv;
        fin >> temp.nx >> temp.ny >> temp.nz;

        vertices[i].position = Vector3(temp.x, temp.y, temp.z);
        vertices[i].textureCoords = Vector2(temp.tu, temp.tv);        

        indices[i] = i;
    }

    // Close the model file.
    fin.close();

    // Make the buffers
    m_vertexBuffer = Buffers::GenVertexBuffer(sizeof(SkyDomeVertex) * vertexCount, vertices);
    m_indexBuffer = Buffers::GenIndexBuffer(sizeof(unsigned int) * indCount, indices);

    delete[] indices;
    delete[] vertices;

    // Make the texture
    unsigned char* texture = new unsigned char[TEXTURE_SIZE * TEXTURE_SIZE * 3];
    for (int i = 0; i < TEXTURE_SIZE * TEXTURE_SIZE; i++)
    {
        texture[3 * i] = (unsigned char)Random::Rnd(255);
        texture[3 * i + 1] = 0;
        texture[3 * i + 2] = 0;
    }
    
    glGenTextures(1, &randTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glBindTexture(GL_TEXTURE_2D, randTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, TEXTURE_SIZE, TEXTURE_SIZE, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void SkyDome::Render()
{
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);

    // Bind side windows
    glBindTexture(GL_TEXTURE_2D, randTexture);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(SkyDomeVertex), NULL);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, sizeof(SkyDomeVertex), (unsigned char*)NULL + (3 * sizeof(float)));


    glDrawElements(GL_TRIANGLES, indCount, GL_UNSIGNED_INT, 0);

    for (int i = 0; i < 2; i++)
    {
        glDisableVertexAttribArray(i);
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}