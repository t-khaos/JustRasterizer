#pragma once


#include "../Math/Vector.h"

struct Vertex{
    Point3f position;
    Vector3f normal;
    Point2f uv;
    float w;
};