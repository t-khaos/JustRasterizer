
#include <fstream>
#include <iostream>
#include "Film.h"
#include "../Model/Texture.h"

void Film::Develop(std::string fileName) {
    //屏幕坐标系起点为坐下而tga起点为左上，故需要翻转
    image->flip_vertically();

    //高斯模糊
    Texture frame(image, 7);
    for (int i = 0; i < frame.width; i++) {
        for (int j = 0; j < frame.height; j++) {
            Point2f uv(float(i) / frame.width, floor(j) / frame.height);
            Color3f color = frame.GetColorGaussianBlur(uv, 0);
            image->set(i, j, TGAColor(color.r, color.g, color.b));
        }
    }

    frame.image->write_tga_file(fileName.c_str());
    std::cout << "Done." << std::endl;
}
