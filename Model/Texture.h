#pragma once

#include <memory>
#include <vector>
#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Vector.h"
#include "ConvKernel.h"

struct Texture {
    int width, height;

    TGAImage::Format format;

    std::shared_ptr<TGAImage> image;

    std::vector<std::shared_ptr<TGAImage>> mipmap;

    Texture(const std::string &fileName, int _width, TGAImage::Format _format, int level);

    void GenMipmap(std::shared_ptr<TGAImage> &current, ConvKernel<float> &kernel, int level);

    TGAColor GetColor(const Point2f &uv, int level);

    TGAColor GetColorBilinear(const Point2f &uv, int level);
};

