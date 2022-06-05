#pragma once

#include <vector>
#include <string>

#include "../Include/JustMath/Vector.h"
#include "../Include/TGA/tgaimage.h"


struct Film {
    int width;
    int height;
    TGAImage image;

    Film(const int &_w, const int &_h)
            : width(_w), height(_h){
        image = TGAImage(width, height, TGAImage::RGB);
    }

    void Develop(std::string fileName);
};