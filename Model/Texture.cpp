
#include "Texture.h"

Texture::Texture(const std::string &fileName, int _width, TGAImage::Format _format, int level = 0) {
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

void Texture::GenMipmap(std::shared_ptr<TGAImage> &current, ConvKernel<float> &kernel, int level) {
    for (int j = 0, t = 0; j < mipmap[level - 1]->get_height() && t < current->get_height(); j += 2, t++) {
        for (int i = 0, s = 0; i < mipmap[level - 1]->get_width() && s < current->get_width(); i += 2, s++) {
            //取得平均值写入当前层级的mipmap
            Color3i color;
            TGAColor pixel;
            for (auto k: kernel.matrix) {
                auto offset = kernel.Next();
                pixel = mipmap[level - 1]->get(i + offset.x, j + offset.y);
                color += Color3i(pixel.r * k, pixel.g * k, pixel.b * k);
            }
            color /= kernel.Size();
            pixel = TGAColor(color.r, color.g, color.b);
            current->set(s, t, pixel);
        }
    }
}

TGAColor Texture::GetColor(const Point2f &uv, int level = 0) {
    //检查层级是否超过mipmap
    assert(level < mipmap.size());
    //四舍五入
    int x = uv.u * (width >> level) + 0.5f;
    int y = uv.v * (height >> level) + 0.5f;
    return mipmap[level]->get(x, y);
}

TGAColor Texture::GetColorBilinear(const Point2f &uv, int level = 0) {
    //检查层级是否超过mipmap
    assert(level < mipmap.size());

    //纹理坐标
    float x = uv.u * (width >> level);
    float y = uv.v * (height >> level);

    //中心坐标
    int centerX = (x - int(x)) > 0.5f ? std::ceil(x) : std::floor(x);
    int centerY = (y - int(y)) > 0.5f ? std::ceil(y) : std::floor(y);

    //左下纹理坐标
    float s = x - (centerX - 0.5f);
    float t = y - (centerY - 0.5f);

    //按offset顺序重新排列
    std::vector<float> matrix = {
            (1 - s) * (1 - t), s * (1 - t),
            (1 - s) * t, s * t
    };
    //正常插值顺序
    //00: (1 - s) * t,          01: s * t,
    //10: (1 - s) * (1 - t),    11: s * (1 - t)

    //卷积操作
    ConvKernel kernel(matrix);
    Color3f color;
    TGAColor pixel;
    for (auto &k: kernel.matrix) {
        auto offset = kernel.Next();
        offset.x = offset.x ? 1 : -1;
        offset.y = offset.y ? 1 : -1;
        pixel = mipmap[level]->get(centerX + offset.x * 0.5f, centerY + offset.y * 0.5f);
        color += Color3f(pixel.r * k, pixel.g * k, pixel.b * k);
    }
    return TGAColor(color.r, color.g, color.b);
}