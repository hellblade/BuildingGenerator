#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "TextureShader.h"

bool TextureShader::Initilise()
{
    return InitializeShader("shaders/TexturedVS.fx", "shaders/TexturedPS.fx");
}

void TextureShader::BindLocations()
{
    glBindAttribLocation(m_shaderProgram, 0, "position");
    glBindAttribLocation(m_shaderProgram, 1, "texCoord");
}

void TextureShader::SetParam(glm::fmat4x4 matrix)
{
    GLint location;

    // Set the world matrix in the vertex shader.
    location = glGetUniformLocation(m_shaderProgram, "mat");
    if (location == -1)
    {
        return;
    }
    glUniformMatrix4fv(location, 1, false, glm::value_ptr(matrix));

    location = glGetUniformLocation(m_shaderProgram, "Texture");
    if (location == -1)
    {
        return;
    }
    glUniform1i(location, 0);
}