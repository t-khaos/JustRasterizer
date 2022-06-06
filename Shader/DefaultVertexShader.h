#pragma once


#include "../Common/VertexShader.h"

struct DefaultVertexShader : VertexShader {

    DefaultVertexShader() {}

    virtual void VertexShading(Vertex &vertex) override;
};