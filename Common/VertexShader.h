#pragma once

#include "../Include/JustMath/Vector.h"
#include "../Include/JustMath/Matrix.h"
#include "Vertex.h"

struct VertexShader {
    Matrix4f MVP;

    virtual Vertex VertexShading(Vertex &vertex) = 0;
};