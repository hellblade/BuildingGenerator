#include <stdio.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "SkyDomeShader.h"

bool SkyDomeShader::Initilise()
{
    return InitializeShader("shaders/SkyDomeVS.fx", "shaders/SkyDomePS.fx");
}

void SkyDomeShader::BindLocations()
{
    glBindAttribLocation(m_shaderProgram, 0, "position");
    glBindAttribLocation(m_shaderProgram, 1, "txtCoords");
}

void SkyDomeShader::SetParam(glm::fmat4x4 world, glm::fmat4x4 view, glm::fmat4x4 proj, float time)
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

    location = glGetUniformLocation(m_shaderProgram, "xTime");
    if (location == -1)
    {
        printf("Uniform location failed to be located\n");
        return;
    }
    glUniform1f(location, time);
}
