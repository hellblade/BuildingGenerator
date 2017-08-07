#pragma once

#include <glm/glm.hpp>
#include <ShaderFile.h>
#include <MathHelper.h>

class BuildingShader : public ShaderFile
{
public:   
    virtual bool Initilise();
    void SetParam(glm::fmat4x4 world, glm::fmat4x4 view, glm::fmat4x4 proj, Vector3 lightDir, Vector3 ambient);

protected:
    virtual void BindLocations();
};
