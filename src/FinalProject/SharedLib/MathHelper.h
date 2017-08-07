#pragma once

#include <stdlib.h>
#include <math.h>

typedef struct Vector2
{
    float x;
    float y;    

    Vector2();
    Vector2(float x, float y);

    float Length();
    void Normalise();
    Vector2 GetNormalisedVector();

    friend Vector2 operator+(const Vector2 &v1, const Vector2 &v2);
    friend Vector2 operator-(const Vector2 &v1, const Vector2 &v2);
    friend Vector2 operator-(const Vector2 &v1);
    friend bool operator== (const Vector2 &v1, const Vector2 &v2);
    friend bool operator!= (const Vector2 &v1, const Vector2 &v2);
} Vector2;

typedef struct Vector3
{
    float x;
    float y;
    float z;    

    Vector3();
    Vector3(float x, float y, float z);

    float Length();
    void Normalise();
    Vector3 GetNormalisedVector();

    friend Vector3 operator+(const Vector3 &v1, const Vector3 &v2);
    friend Vector3 operator-(const Vector3 &v1, const Vector3 &v2);
    friend Vector3 operator-(const Vector3 &v1);
    friend bool operator== (const Vector3 &v1, const Vector3 &v2);
    friend bool operator!= (const Vector3 &v1, const Vector3 &v2);
} Vector3;

typedef struct Rectangle3D
{
    Vector2 topLeft;
    float height;
    Vector2 bottomRight;
    Vector3 points[8];
    float width;
    float depth;

    Rectangle3D(float x, float y, float width, float depth, float height);
    Rectangle3D();

    bool Intersects(Rectangle3D other);
} Rectangle3D;

inline Vector3 CrossProduct(const Vector3 &pt1, const Vector3 &pt2)
{
    Vector3 result;
    result.x = (pt1.y * pt2.z) - (pt1.z * pt2.y);
    result.y = (pt1.z * pt2.x) - (pt1.x * pt2.z);
    result.z = (pt1.x * pt2.y) - (pt1.y * pt2.x);

    return result;
}

Vector3 GetTriangleNormal(const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);

template <class T>
inline T Min(T first, T second)
{
    if (first < second)
        return first;
    return second;
}

template <class T>
inline T Max(T first, T second)
{
    if (first > second)
        return first;
    return second;
}

template <class T>
inline T Clamp(T value, T min, T max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

inline int Mod(int n, int m)
{
    if (n >= 0)
        return n % m;

    if (n > -m)
    {
        return (n + m) % m;
    }

    return abs(n % m);
}

inline bool InsideRectangle(float x, float y, float rectX, float rectY, float rectWidth, float rectHeight)
{
    return (rectX <= x && x <= rectX + rectWidth) && (rectY <= y && y <= rectY + rectHeight);
}