
#include <fstream>
#include <iostream>
#include "Film.h"

void Film::Develop(std::string fileName) {
    //屏幕坐标系起点为坐下而tga起点为左上，故需要翻转
    image.flip_vertically();
    image.write_tga_file(fileName.c_str());
    std::cout<<"Done."<<std::endl;
}
