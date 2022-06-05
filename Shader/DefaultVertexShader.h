#pragma once


#include "../Common/VertexShader.h"

struct DefaultVertexShader : VertexShader {

    DefaultVertexShader() {}

    virtual Vertex VertexShading(Vertex &vertex) override;

};