#pragma once

#include <vector>
#include <string>

#include "../Include/JustMath/Vector.h"
#include "../Include/TGA/tgaimage.h"


struct Film {
    int width;
    int height;
    std::vector<Color3f> pixels;
    TGAImage image;

    Film(const int &_w, const int &_h)
            : width(_w), height(_h){
        pixels = std::vector<Color3f>(width*height,Color3f(0.0f));
        image = TGAImage(width, height, TGAImage::RGB);
    }

    void Develop(std::string fileName);
};