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

    Texture(const std::string &fileName, int _width, TGAImage::Format _format, int level = 0) {
        //贴图长宽必须是2的n次幂
        assert((_width & (_width - 1)) == 0);

        //纹理初始化
        format = _format;
        width = _width;
        height = _width;
        image = std::make_shared<TGAImage>(width, height, format);

        //mipmap初始化
        mipmap = std::vector<std::shared_ptr<TGAImage>>(level + 1);
        mipmap[0] = image;
        if (!image->read_tga_file(fileName.c_str())) {
            std::cerr << "Fail to load texture." << std::endl;
        }
        std::cout << "Succeed to load texture." << std::endl;
        image->flip_vertically();

        //mipmap层级最高不得使长宽小于等于32
        while (width >> (level + 1) <= 32 && level > 0)
            level--;

        //生成mipmap
        if (level) {
            //生成卷积核
            std::vector<float> matrix = {
                    1, 1,
                    1, 1
            };
            ConvKernel kernel(matrix);

            //生成mipmap的每一层级
            for (int l = 0; l < level; l++) {
                mipmap[l + 1] = std::make_shared<TGAImage>(width >> (l + 1), height >> (l + 1), format);
                GenMipmap(mipmap[l + 1], kernel, l + 1);
            }
        }
    }

    void GenMipmap(std::shared_ptr<TGAImage> &current, ConvKernel &kernel, int level) {
        for (int j = 0, t = 0; j < mipmap[level - 1]->get_height() && t < current->get_height(); j += 2, t++) {
            for (int i = 0, s = 0; i < mipmap[level - 1]->get_width() && s < current->get_width(); i += 2, s++) {
                //取得平均值写入当前层级的mipmap
                Color3i color;
                TGAColor pixel;
                for (auto k: kernel.matrix) {
                    auto [x, y] = kernel.Next();
                    pixel = mipmap[level - 1]->get(i + x, j + y);
                    color += Color3i(pixel.r * k, pixel.g * k, pixel.b * k);
                }
                color /= kernel.Size();
                pixel = TGAColor(color.r, color.g, color.b);
                current->set(s, t, pixel);
            }
        }
    }

    TGAColor GetColorByUV(const Point2f &uv, int level = 0) {
        //检查层级是否超过mipmap
        assert(level<mipmap.size());
        int w = width >> level;
        int h = height >> level;
        return mipmap[level]->get(uv.s * w, uv.t * h);
    }
};

