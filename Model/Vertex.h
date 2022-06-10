#pragma once

#include "../Include/JustMath/Vector.h"

struct Vertex{
    Point3f position;
    Point3f worldPos;
    Vector3f normal;
    Point2f uv;
    float w;
};