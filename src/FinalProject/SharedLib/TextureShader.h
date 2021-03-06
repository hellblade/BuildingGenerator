#pragma once

#include <glm/glm.hpp>
#include "ShaderFile.h"

class TextureShader : public ShaderFile
{
public:
    virtual bool Initilise();
    void SetParam(glm::fmat4x4);

protected:    
    virtual void BindLocations();
};