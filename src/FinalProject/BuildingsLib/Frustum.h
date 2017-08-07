#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Frustum;
#include "QuadTree.h"

struct Plane
{
    float a, b, c, d;
};

class Frustum
{
private:
    Plane planes[6];

public:

    void SetPlanes(glm::fmat4x4 matrix);
    bool InFrustum(Rectangle3D rect);
};