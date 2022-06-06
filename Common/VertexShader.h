#pragma once

#include "../Include/JustMath/Vector.h"
#include "../Include/JustMath/Matrix.h"
#include "Vertex.h"
#include "Shader.h"

struct VertexShader : Shader {
    Matrix4f MVP;
    Matrix4f N;
    Vertex vertex;

    virtual void VertexShading(Vertex &vertex) = 0;
};