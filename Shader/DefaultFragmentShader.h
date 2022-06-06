#pragma once

#include "../Common/FragmenShader.h"

struct DefaultFragmentShader : FragmentShader {

    DefaultFragmentShader() {}

    virtual TGAColor FragmentShading(Vertex &vertex) override;
};
