#pragma once

#include <memory>
#include <vector>
#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Vector.h"
#include "ConvKernel.h"

struct Texture {
    int width, height;

    int level;

    TGAImage::Format format;

    std::shared_ptr<TGAImage> image;

    std::vector<std::shared_ptr<TGAImage>> mipmap;

    Texture(const std::string &fileName, int _width, TGAImage::Format _format, int _level);
    Texture(std::shared_ptr<TGAImage> &other, int _level);

    void GenMipmap(std::shared_ptr<TGAImage> &current, ConvKernel<float> &kernel, int _level);

    TGAColor GetColor(const Point2f &uv, int _level);

    TGAColor GetColorBilinear(const Point2f &uv, int _level);

    Color3f GetColorGaussianBlur(const Point2f &uv, int _level);

    void Texture::InitMipMap(int _level);

    float GetWeight(Color3f &color, int _level);
};

