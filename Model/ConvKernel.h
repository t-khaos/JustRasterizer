#pragma once

#include <vector>
#include <cassert>

struct ConvKernel {
    std::vector<float> matrix;
    int index;
    union {
        int width;
        int height;
    };

    ConvKernel(const std::vector<float> &_mat) : matrix(_mat) {
        index = 0;
        width = sqrt(matrix.size());
        //卷积核尺寸必须长宽相等
        assert(width * height == matrix.size());
    }

    ConvKernel(const ConvKernel &_kernel) {
        index = 0;
        matrix = _kernel.matrix;
        width = _kernel.width;
        //卷积核尺寸必须长宽相等
        assert(width * height == matrix.size());
    }

    int Size() {
        return matrix.size();
    }

    std::tuple<int, int> Next() {
        int x = index % width;
        int y = index / height;
        if (++index >= matrix.size())
            index = 0;
        return {x, y};
    }
};