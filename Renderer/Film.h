#pragma once

#include <vector>
#include <string>

#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Global.h"


struct Film {
    int width;
    int height;
    TGAImage image;
    std::vector<float> zBuffer;

    Film(const int &_w, const int &_h)
            : width(_w), height(_h) {
        image = TGAImage(width, height, TGAImage::RGB);
        zBuffer = std::vector<float>(width * height, MAX_FLOAT);
    }

    void Develop(std::string fileName);
};