#include "MathHelper.h"

Vector3::Vector3()
{
    x = 0.0f;
    y = 0.0f;
    z = 0.0f;
}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

float Vector3::Length()
{
    return sqrt(x * x + y * y + z * z);
}

void Vector3::Normalise()
{
    float magnitude = Length();
    if (magnitude == 0.0f)
    {
        x = y = z = 0.0f;
    }
    else
    {
        x /= magnitude;
        y /= magnitude;
        z /= magnitude;
    }
}

Vector3 Vector3::GetNormalisedVector()
{
    Vector3 result;

    float magnitude = Length();

    if (magnitude == 0)
    {
        result.x = result.y = result.z = 0.0f;
    }
    else    
    {
        result.x = x / magnitude;
        result.y = y / magnitude;
        result.z = z / magnitude;
    }    

    return result;
}

Vector3 operator+(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}

Vector3 operator-(const Vector3 &v1, const Vector3 &v2)
{
    return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}

Vector3 operator-(const Vector3 &v1)
{
    return Vector3(-v1.x, -v1.y, -v1.z);
}

Vector3 GetTriangleNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
    Vector3 line1 = v3 - v1;
    Vector3 line2 = v2 - v1;
    Vector3 cross = CrossProduct(line2, line1);
    cross.Normalise();
    return cross;
}

bool operator== (const Vector2 &v1, const Vector2 &v2)
{
    return (v1.x == v2.x && v2.y == v1.y);
}

bool operator!= (const Vector2 &v1, const Vector2 &v2)
{
    return !(v1 == v2);
}

Vector2::Vector2()
{
    x = 0.0f;
    y = 0.0f;
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;    
}

float Vector2::Length()
{
    return sqrt(x * x + y * y);
}

void Vector2::Normalise()
{
    float magnitude = Length();
    if (magnitude == 0.0f)
    {
        x = y = 0.0f;
    }
    else
    {
        x /= magnitude;
        y /= magnitude;        
    }
}

Vector2 Vector2::GetNormalisedVector()
{
    Vector2 result;

    float magnitude = Length();

    if (magnitude == 0)
    {
        result.x = result.y = 0.0f;
    }
    else
    {
        result.x = x / magnitude;
        result.y = y / magnitude;        
    }

    return result;
}

Vector2 operator+(const Vector2 &v1, const Vector2 &v2)
{
    return Vector2(v1.x + v2.x, v1.y + v2.y);
}

Vector2 operator-(const Vector2 &v1, const Vector2 &v2)
{
    return Vector2(v1.x - v2.x, v1.y - v2.y);
}

Vector2 operator-(const Vector2 &v1)
{
    return Vector2(-v1.x, -v1.y);
}

bool operator== (const Vector3 &v1, const Vector3 &v2)
{
    return (v1.x == v2.x && v2.y == v1.y && v1.z == v2.z);
}

bool operator!= (const Vector3 &v1, const Vector3 &v2)
{
    return !(v1 == v2);
}

Rectangle3D::Rectangle3D(float x, float z, float width, float depth, float h)
{
    topLeft = Vector2(x, z);
    bottomRight = Vector2(x + width, z + depth);

    points[0] = Vector3(x, 0, z);
    points[1] = Vector3(x, h, z);

    points[2] = Vector3(x + width, 0, z);
    points[3] = Vector3(x + width, h, z);

    points[4] = Vector3(x, 0, z + depth);
    points[5] = Vector3(x, h, z + depth);

    points[6] = Vector3(x + width, 0, z + depth);
    points[7] = Vector3(x + width, h, z + depth);

    height = h;
    this->width = width;
    this->depth = depth;
}

Rectangle3D::Rectangle3D()
{

}

bool Rectangle3D::Intersects(Rectangle3D other)
{
    return (topLeft.x <= other.bottomRight.x && other.topLeft.x <= bottomRight.x && topLeft.y <= other.bottomRight.y && other.topLeft.y <= bottomRight.y);
}