#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ColourShader.h"

bool ColourShader::Initilise()
{
    return InitializeShader("shaders/ColourVS.fx", "shaders/ColourPS.fx");
}

void ColourShader::BindLocations()
{
    glBindAttribLocation(m_shaderProgram, 0, "position");
    glBindAttribLocation(m_shaderProgram, 1, "colour");
}

void ColourShader::SetParam(glm::fmat4x4 matrix)
{
    GLint location;

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "mat");
    if (location == -1)
    {
        return;
    }
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));
} 
