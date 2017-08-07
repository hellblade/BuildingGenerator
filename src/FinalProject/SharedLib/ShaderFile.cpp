#include <stdio.h>
#include <iostream>
#include <fstream>
#include <GL/glew.h>
#include "ShaderFile.h"

void ShaderFile::SetShader()
{
    glUseProgram(m_shaderProgram);
}

void ShaderFile::Shutdown()
{
    // Detach the vertex and fragment shaders from the program.
    glDetachShader(m_shaderProgram, m_vertexShader);
    glDetachShader(m_shaderProgram, m_fragmentShader);

    // Delete the vertex and fragment shaders.
    glDeleteShader(m_vertexShader);
    glDeleteShader(m_fragmentShader);

    // Delete the shader program.
    glDeleteProgram(m_shaderProgram);
}

bool ShaderFile::InitializeShader(char* vertexShaderFile, char* pixelShaderFile)
{
    const char* vertexShaderBuffer;
    const char* fragmentShaderBuffer;
    int status;

    // Load the vertex shader source file into a text buffer.
    vertexShaderBuffer = LoadShaderSourceFile(vertexShaderFile);
    if (!vertexShaderBuffer)
    {
        return false;
    }

    // Load the fragment shader source file into a text buffer.
    fragmentShaderBuffer = LoadShaderSourceFile(pixelShaderFile);
    if (!fragmentShaderBuffer)
    {
        return false;
    }

    // Create a vertex and fragment shader object.
    m_vertexShader = glCreateShader(GL_VERTEX_SHADER);
    m_fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Copy the shader source code strings into the vertex and fragment shader objects.
    glShaderSource(m_vertexShader, 1, &vertexShaderBuffer, NULL);
    glShaderSource(m_fragmentShader, 1, &fragmentShaderBuffer, NULL);

    // Release the vertex and fragment shader buffers.
    delete[] vertexShaderBuffer;
    vertexShaderBuffer = 0;

    delete[] fragmentShaderBuffer;
    fragmentShaderBuffer = 0;

    // Compile the shaders.
    glCompileShader(m_vertexShader);
    
    glCompileShader(m_fragmentShader);

    // Check to see if the vertex shader compiled successfully.
    glGetShaderiv(m_vertexShader, GL_COMPILE_STATUS, &status);
    if (status != 1)
    {
        // If it did not compile then write the syntax error message out to a text file for review.
        char info[250];
        int count;
        glGetShaderInfoLog(m_vertexShader, 250, &count, info);
        printf("Failed to Compile VS - %s\n", info);
        return false;
    }

    // Check to see if the fragment shader compiled successfully.
    glGetShaderiv(m_fragmentShader, GL_COMPILE_STATUS, &status);
    if (status != 1)
    {
        // If it did not compile then write the syntax error message out to a text file for review.
        char info[250];
        int count;
        glGetShaderInfoLog(m_fragmentShader, 250, &count, info);
        printf("Failed to compile PS - %s \n", info);
        return false;
    }

    // Create a shader program object.
    m_shaderProgram = glCreateProgram();

    // Attach the vertex and fragment shader to the program object.
    glAttachShader(m_shaderProgram, m_vertexShader);
    glAttachShader(m_shaderProgram, m_fragmentShader);

    // Bind the shader input variables.
    BindLocations();

    // Link the shader program.
    glLinkProgram(m_shaderProgram);

    // Check the status of the link.
    glGetProgramiv(m_shaderProgram, GL_LINK_STATUS, &status);
    if (status != 1)
    {
        // If it did not link then write the syntax error message out to a text file for review.
        printf("Failed to link\n");
        return false;
    }

    return true;
}

char* ShaderFile::LoadShaderSourceFile(char* filename)
{
    std::ifstream fin;
    int fileSize;
    char input;
    char* buffer;


    // Open the shader source file.
    fin.open(filename);

    // If it could not open the file then exit.
    if (fin.fail())
    {
        return 0;
    }

    // Initialize the size of the file.
    fileSize = 0;

    // Read the first element of the file.
    fin.get(input);

    // Count the number of elements in the text file.
    while (!fin.eof())
    {
        fileSize++;
        fin.get(input);
    }

    // Close the file for now.
    fin.close();

    // Initialize the buffer to read the shader source file into.
    buffer = new char[fileSize + 1];
    if (!buffer)
    {
        return 0;
    }

    // Open the shader source file again.
    fin.open(filename);

    // Read the shader text file into the buffer as a block.
    fin.read(buffer, fileSize);

    // Close the file.
    fin.close();

    // Null terminate the buffer.
    buffer[fileSize] = '\0';

    return buffer;
}

bool ShaderFile::Initilise()
{
    return false;
}

void ShaderFile::BindLocations()
{

}