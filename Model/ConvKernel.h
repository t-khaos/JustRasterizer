#pragma once

#include <vector>
#include <cassert>

template<typename T>
struct ConvKernel {
    std::vector<T> matrix;
    int index;
    int valid;
    union {
        int width;
        int height;
    };

    ConvKernel(const std::vector<T> &_mat) : matrix(_mat) {
        index = 0;
        width = sqrt(matrix.size());
        valid = matrix.size();
        //卷积核尺寸必须长宽相等
        assert(width * height == matrix.size());
    }

    ConvKernel(const ConvKernel &_kernel) {
        index = 0;
        matrix = _kernel.matrix;
        width = _kernel.width;
        valid = matrix.size();
        //卷积核尺寸必须长宽相等
        assert(width * height == matrix.size());
    }

    int Size() {
        return matrix.size();
    }

    void Init(){
        index=0;
    }

    Int2 Next() {
        int y = index / height;
        int x = index % width;
        if (++index >= matrix.size())
            index = 0;
        return {x, y};
    }
};