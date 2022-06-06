#pragma once

#include "../Include/JustMath/Vector.h"
#include "../Include/JustMath/Matrix.h"
#include "Vertex.h"
#include "Shader.h"

struct VertexShader : Shader {
    Matrix4f MVP;

    virtual void VertexShading(Vertex &vertex) = 0;
};