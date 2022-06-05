

#include <fstream>
#include <iostream>
#include "Film.h"
#include "../Include/JustMath/Math.h"

void Film::Develop(std::string fileName) {
    for (int row = 0; row < width; row++) {
        for (int col = 0; col < height; col++) {
            Color3f color = pixels[col * width + row];
            image.set(row, col, TGAColor(ToRGB(color.x), ToRGB(color.y), ToRGB(color.z)));
        }
    }
    //屏幕坐标系起点为坐下而tga起点为左上，故需要翻转
    image.flip_vertically();
    image.write_tga_file(fileName.c_str());
    std::cout<<"Done."<<std::endl;
}