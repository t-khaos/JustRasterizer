#pragma once

#include "../Include/JustMath/Vector.h"
#include "../Include/TGA/tgaimage.h"

struct FragmentShader {
    Vector3f normal;
    Point2i uv;
    Vector3f lightDir;
    std::shared_ptr<TGAImage> diffuseMap;
    std::shared_ptr<TGAColor> normalMap;
    std::shared_ptr<TGAColor> specularMap;

    virtual TGAColor FragmentShading() = 0;
};