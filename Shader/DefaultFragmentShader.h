#pragma once

#include "../Common/FragmenShader.h"

struct DefaultFragmentShader : FragmentShader {

    DefaultFragmentShader() {
        lightDir = Vector3f(0, 0, 1);
    }

    virtual TGAColor FragmentShading() override;
};