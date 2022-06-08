#pragma once

#include <vector>
#include <string>

#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Global.h"


struct Film {
    int width;
    int height;
    TGAImage image;
    std::vector<float> depthBuffer;
    std::vector<TGAColor> frameBuffer;

    std::vector<float> depthBufferMSAA2x2;
    std::vector<TGAColor> frameBufferMSAA2x2;

    Film(const int &_w, const int &_h)
            : width(_w), height(_h) {
        image = TGAImage(width, height, TGAImage::RGB);
        depthBuffer = std::vector<float>(width * height, MAX_FLOAT);
        frameBuffer = std::vector<TGAColor>(width * height);
        depthBufferMSAA2x2 = std::vector<float>(width * height * 2 * 2, MAX_FLOAT);
        frameBufferMSAA2x2 = std::vector<TGAColor>(width * height * 2 * 2);
    }

    void Develop(std::string fileName);
};