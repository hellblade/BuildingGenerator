#pragma once

class ShaderFile
{

public:
    virtual bool Initilise();
    void Shutdown();
    void SetShader();
    
protected:
    bool InitializeShader(char*, char*);
    virtual void BindLocations();

    unsigned int m_vertexShader;
    unsigned int m_fragmentShader;
    unsigned int m_shaderProgram;

private:
    char* LoadShaderSourceFile(char*);
};