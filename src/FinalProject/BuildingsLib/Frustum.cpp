#include <math.h>
#include <MathHelper.h>
#include "Frustum.h"

struct Matrix4x4
{
    // The elements of the 4x4 matrix are stored in
    // column-major order (see "OpenGL Programming Guide",
    // 3rd edition, pp 106, glLoadMatrix).
    float _11, _21, _31, _41;
    float _12, _22, _32, _42;
    float _13, _23, _33, _43;
    float _14, _24, _34, _44;
};

void NormalizePlane(Plane & plane)
{
    float mag;
    mag = sqrt(plane.a * plane.a + plane.b * plane.b + plane.c * plane.c);
    plane.a = plane.a / mag;
    plane.b = plane.b / mag;
    plane.c = plane.c / mag;
    plane.d = plane.d / mag;
}

float DistanceToPoint(const Plane & plane, const Vector3& pt)
{
    return plane.a*pt.x + plane.b*pt.y + plane.c*pt.z + plane.d;
}

enum Halfspace
{
    NEGATIVE = -1,
    ON_PLANE = 0,
    POSITIVE = 1
};


Halfspace ClassifyPoint(const Plane & plane, const Vector3& pt)
{
    float d;
    d = plane.a*pt.x + plane.b*pt.y + plane.c*pt.z + plane.d;
    if (d < 0) return NEGATIVE;
    if (d > 0) return POSITIVE;
    return ON_PLANE;
}

bool Frustum::InFrustum(Rectangle3D rect)
{
    // Look at each plane
    for (int p = 0; p < 6; p++)
    {
        bool inSide = false;
        for (int i = 0; i < 8; i++)
        {
            if (ClassifyPoint(planes[p], rect.points[i]) != NEGATIVE)
            {
                inSide = true;
                break;
            }
        }

        if (!inSide)
            return false;
    }

    return true;
}

void Frustum::SetPlanes(glm::fmat4x4 matrix)
{
    Matrix4x4 comboMatrix;    
    comboMatrix._11 = matrix[0][0];
    comboMatrix._21 = matrix[0][1];
    comboMatrix._31 = matrix[0][2];
    comboMatrix._41 = matrix[0][3];

    comboMatrix._12 = matrix[1][0];
    comboMatrix._22 = matrix[1][1];
    comboMatrix._32 = matrix[1][2];
    comboMatrix._42 = matrix[1][3];

    comboMatrix._13 = matrix[2][0];
    comboMatrix._23 = matrix[2][1];
    comboMatrix._33 = matrix[2][2];
    comboMatrix._43 = matrix[2][3];

    comboMatrix._14 = matrix[3][0];
    comboMatrix._24 = matrix[3][1];
    comboMatrix._34 = matrix[3][2];
    comboMatrix._44 = matrix[3][3];

    // Left clipping plane
    planes[0].a = comboMatrix._41 + comboMatrix._11;
    planes[0].b = comboMatrix._42 + comboMatrix._12;
    planes[0].c = comboMatrix._43 + comboMatrix._13;
    planes[0].d = comboMatrix._44 + comboMatrix._14;
    // Right clipping plane
    planes[1].a = comboMatrix._41 - comboMatrix._11;
    planes[1].b = comboMatrix._42 - comboMatrix._12;
    planes[1].c = comboMatrix._43 - comboMatrix._13;
    planes[1].d = comboMatrix._44 - comboMatrix._14;
    // Top clipping plane
    planes[2].a = comboMatrix._41 - comboMatrix._21;
    planes[2].b = comboMatrix._42 - comboMatrix._22;
    planes[2].c = comboMatrix._43 - comboMatrix._23;
    planes[2].d = comboMatrix._44 - comboMatrix._24;
    // Bottom clipping plane
    planes[3].a = comboMatrix._41 + comboMatrix._21;
    planes[3].b = comboMatrix._42 + comboMatrix._22;
    planes[3].c = comboMatrix._43 + comboMatrix._23;
    planes[3].d = comboMatrix._44 + comboMatrix._24;
    // Near clipping plane
    planes[4].a = comboMatrix._41 + comboMatrix._31;
    planes[4].b = comboMatrix._42 + comboMatrix._32;
    planes[4].c = comboMatrix._43 + comboMatrix._33;
    planes[4].d = comboMatrix._44 + comboMatrix._34;
    // Far clipping plane
    planes[5].a = comboMatrix._41 - comboMatrix._31;
    planes[5].b = comboMatrix._42 - comboMatrix._32;
    planes[5].c = comboMatrix._43 - comboMatrix._33;
    planes[5].d = comboMatrix._44 - comboMatrix._34;
    // Normalize the plane equations, if requested    
        NormalizePlane(planes[0]);
        NormalizePlane(planes[1]);
        NormalizePlane(planes[2]);
        NormalizePlane(planes[3]);
        NormalizePlane(planes[4]);
        NormalizePlane(planes[5]);    
}