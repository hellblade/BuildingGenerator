#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "BuildingShader.h"

bool BuildingShader::Initilise()
{
    return InitializeShader("shaders/BuildingVS.fx", "shaders/BuildingPS.fx");
}

void BuildingShader::BindLocations()
{
    glBindAttribLocation(m_shaderProgram, 0, "position");
    glBindAttribLocation(m_shaderProgram, 1, "normal");
    glBindAttribLocation(m_shaderProgram, 2, "colour");    
    glBindAttribLocation(m_shaderProgram, 3, "textureCoords");
    glBindAttribLocation(m_shaderProgram, 4, "topTexture");
}

void BuildingShader::SetParam(glm::fmat4x4 world, glm::fmat4x4 view, glm::fmat4x4 proj, Vector3 lightDir, Vector3 ambient)
{
    GLint location;

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "worldMatrix");

    if (location == -1)
    {
        printf("ERROR: UNIFROM NOT FOUND\n");
        return;
    }
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(world));

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "viewMatrix");
    if (location == -1)
    {
        printf("ERROR: UNIFROM NOT FOUND\n");
        return;
    }
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(view));

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "projectionMatrix");
    if (location == -1)
    {
        printf("ERROR: UNIFROM NOT FOUND\n");
        return;
    }
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(proj));

    location = glGetUniformLocation(m_shaderProgram, "texColour");
    if (location == -1)
    {
        printf("Uniform location failed to be located\n");
        return;
    }
    glUniform1i(location, 0);

    location = glGetUniformLocation(m_shaderProgram, "texNormal");
    if (location == -1)
    {
        printf("Uniform location failed to be located\n");
        return;
    }
    glUniform1i(location, 1);    


    location = glGetUniformLocation(m_shaderProgram, "texRoof");
    if (location == -1)
    {
        printf("Uniform location failed to be located\n");
        return;
    }
    glUniform1i(location, 2);

    location = glGetUniformLocation(m_shaderProgram, "texRoofNormal");
    if (location == -1)
    {
        printf("Uniform location failed to be located\n");
        return;
    }
    glUniform1i(location, 3);


    lightDir.Normalise();

    location = glGetUniformLocation(m_shaderProgram, "lightDir");
    if (location == -1)
    {
        printf("Uniform location failed to be located\n");
        return;
    }
    glUniform3f(location, lightDir.x, lightDir.y, lightDir.z);

    location = glGetUniformLocation(m_shaderProgram, "ambient");
    if (location == -1)
    {
        printf("Uniform location failed to be located\n");
        return;
    }
    glUniform3f(location, ambient.x, ambient.y, ambient.z);
}
