#pragma once

#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Vector.h"
#include "Shader.h"
#include "Vertex.h"


struct FragmentShader : Shader {
    Vector3f normal;
    Point2i uv;
    Vector3f lightDir;
    std::shared_ptr<TGAImage> diffuseMap;
    std::shared_ptr<TGAColor> normalMap;
    std::shared_ptr<TGAColor> specularMap;

    virtual TGAColor FragmentShading() = 0;
};

struct DefaultFragmentShader : FragmentShader {

    DefaultFragmentShader() {}

    virtual TGAColor FragmentShading() override;
};