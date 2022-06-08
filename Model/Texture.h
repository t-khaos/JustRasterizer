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

    void GenMipmap(std::shared_ptr<TGAImage> &current, ConvKernel<float> &kernel, int level) {
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

    TGAColor GetColor(const Point2f &uv, int level = 0) {
        //检查层级是否超过mipmap
        assert(level < mipmap.size());
        //四舍五入
        int x = uv.u * (width >> level) + 0.5f;
        int y = uv.v * (height >> level) + 0.5f;
        return mipmap[level]->get(x, y);
    }

    //双线性插值
    TGAColor GetColorBilinear(const Point2f &uv, int level = 0) {
        //  y ↑
        //    +++++++++++++++++
        //    |  u00  |  u01  |
        //    |   ·   |   ·   |
        //    |+++++++·+++++++|
        //    |   ·   |   ·   |
        //    |  u10  |  u11  |
        //    +++++++++++++++++  → x


        //检查层级是否超过mipmap
        assert(level < mipmap.size());

        auto &current = mipmap[level];

        //纹理坐标
        float x = uv.u * (width >> level);
        float y = uv.v * (height >> level);

        int centerX = (x - int(x)) > 0.5f ? std::ceil(x) : std::floor(x);
        int centerY = (y - int(y)) > 0.5f ? std::ceil(y) : std::floor(y);

        //左下像素纹理坐标
        float s = x - (centerX - 0.5f);
        float t = y - (centerY - 0.5f);

        //按offset顺序重新排列
        std::vector<float> matrix = {
                (1 - s) * (1 - t), (1 - s) * t,
                s * (1 - t), s * t
        };
        //正常插值顺序
        //00: (1 - s) * t,          01: s * t,
        //10: (1 - s) * (1 - t),    11: s * (1 - t)
        float a =  (1 - s) * (1 - t)*100;
        float b = (1 - s)* t*100;
        float c = s * (1 - t)*100;
        float d = s * t*100;
        float num=a+b+c+d;

        ConvKernel kernel(matrix);
        Color3f color;
        TGAColor pixel;
        for (auto &k: kernel.matrix) {
            auto [offsetX, offsetY] = kernel.Next();
            offsetX = offsetX ? 1 : -1;
            offsetY = offsetY ? 1 : -1;
            pixel = current->get(centerX + offsetX * 0.5f, centerY + offsetY * 0.5f);
            color += Color3f(pixel.r * k, pixel.g * k, pixel.b * k);
        }
        return TGAColor(color.r, color.g, color.b);
    }
};

