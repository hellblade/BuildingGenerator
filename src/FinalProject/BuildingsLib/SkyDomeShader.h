#pragma once

#include <glm/glm.hpp>
#include <ShaderFile.h>
#include <MathHelper.h>

class SkyDomeShader : public ShaderFile
{
public:
    virtual bool Initilise();
    void SetParam(glm::fmat4x4 world, glm::fmat4x4 view, glm::fmat4x4 proj, float time);

protected:
    virtual void BindLocations();
};
