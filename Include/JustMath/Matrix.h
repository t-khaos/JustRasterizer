#pragma once

#include <iostream>
#include "Vector.h"
#include "Global.h"
#include "Math.h"

template<size_t ROW, size_t COL, typename T>
struct Matrix {
    T data[ROW][COL];

    inline Matrix() {}

    inline Matrix(const Matrix<ROW, COL, T> &other) {
        for (size_t row = 0; row < ROW; row++) {
            for (size_t col = 0; col < COL; col++)
                data[row][col] = other[row][col];
        }
    }

    inline Matrix(const std::initializer_list<Vector<COL, T>> &u) {
        auto it = u.begin();
        for (size_t i = 0; i < ROW; i++)
            SetRow(i, *it++);
    }

    inline const T *operator[](size_t row) const {
        assert(row < ROW);
        return data[row];
    }

    inline T *operator[](size_t row) {
        assert(row < ROW);
        return data[row];
    }

    //获取行
    Vector<COL, T> GetRow(size_t row) const {
        assert(row < ROW);
        Vector<COL, T> v;
        for (size_t col = 0; col < COL; col++)
            v[col] = data[row][col];
        return v;
    }

    //获取列
    inline Vector<ROW, T> GetCol(size_t col) const {
        assert(col < COL);
        Vector<ROW, T> v;
        for (size_t row = 0; row < ROW; row++)
            v[row] = data[row][col];
        return v;
    }

    //设置行
    inline void SetRow(size_t row, const Vector<COL, T> &v) {
        assert(row < ROW);
        for (size_t i = 0; i < COL; i++)
            data[row][i] = v[i];
    }

    //设置列
    inline void SetCol(size_t col, const Vector<COL, T> &v) {
        assert(col < COL);
        for (size_t i = 0; i < ROW; i++)
            data[i][col] = v[i];
    }

    //删除某行某列
    inline Matrix<ROW - 1, COL - 1, T> GetMinor(size_t r, size_t c) const {
        Matrix<ROW - 1, COL - 1, T> ret;
        for (size_t row = 0; row < ROW - 1; row++) {
            for (size_t col = 0; col < COL - 1; col++)
                ret.data[row][col] = data[row < r ? row : row + 1][col < c ? col : col + 1];
        }
        return ret;
    }

    //转置矩阵
    inline Matrix<COL, ROW, T> Transpose() const {
        Matrix<COL, ROW, T> ret;
        for (size_t row = 0; row < ROW; row++) {
            for (size_t col = 0; col < COL; col++)
                ret.data[col][row] = data[row][col];
        }
        return ret;
    }
};

//获取零矩阵
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> GetZero() {
    Matrix<ROW, COL, T> ret;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++)
            ret.data[row][col] = 0;
    }
}

//获取单位矩阵
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> GetIdentity() {
    Matrix<ROW, COL, T> ret;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++)
            ret.data[row][col] = (row == col) ? 1 : 0;
    }
}

//矩阵运算
//判断相等
template<size_t ROW, size_t COL, typename T>
inline bool operator==(const Matrix<ROW, COL, T> &a, const Matrix<ROW, COL, T> &b) {
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            if (a.data[row][col] != b.data[row][col])
                return false;
        }
    }
    return true;
}

template<size_t ROW, size_t COL, typename T>
inline bool operator!=(const Matrix<ROW, COL, T> &a, const Matrix<ROW, COL, T> &b) {
    return !(a == b);
}

//+a
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator+(Matrix<ROW, COL, T> &a) {
    return a;
}

//-a
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator-(Matrix<ROW, COL, T> &a) {
    Matrix<ROW, COL, T> temp;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            temp.data[row][col] = -a.data[row][col];
        }
    }
    return temp;
}

//a+b
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator+(const Matrix<ROW, COL, T> &a, const Matrix<ROW, COL, T> &b) {
    Matrix<ROW, COL, T> temp;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            temp.data[row][col] = a.data[row][col] + b.data[row][col];
        }
    }
    return temp;
}

//a-b
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator-(const Matrix<ROW, COL, T> &a, const Matrix<ROW, COL, T> &b) {
    Matrix<ROW, COL, T> temp;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            temp.data[row][col] = a.data[row][col] - b.data[row][col];
        }
    }
    return temp;
}

//a*b
template<size_t ROW, size_t COM, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator*(const Matrix<ROW, COM, T> &a, const Matrix<COM, COL, T> &b) {
    Matrix<ROW, COL, T> out;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            out.data[row][col] = Dot(a.GetRow(row), b.GetCol(col));
        }
    }
    return out;
}

//a*k
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator*(const Matrix<ROW, COL, T> &a, T k) {
    Matrix<ROW, COL, T> temp;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            temp.data[row][col] = a.data[row][col] * k;
        }
    }
    return temp;
}

//a/k
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator/(const Matrix<ROW, COL, T> &a, T k) {
    Matrix<ROW, COL, T> temp;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            temp.data[row][col] = a.data[row][col] / k;
        }
    }
    return temp;
}

//k*a
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator*(T k, const Matrix<ROW, COL, T> &a) {
    Matrix<ROW, COL, T> temp;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            temp.data[row][col] = a.data[row][col] * k;
        }
    }
    return temp;
}

//k/a
template<size_t ROW, size_t COL, typename T>
inline Matrix<ROW, COL, T> operator/(T k, const Matrix<ROW, COL, T> &a) {
    Matrix<ROW, COL, T> temp;
    for (size_t row = 0; row < ROW; row++) {
        for (size_t col = 0; col < COL; col++) {
            temp.data[row][col] = k / a.data[row][col];
        }
    }
    return temp;
}

//行向量x矩阵
template<size_t ROW, size_t COL, typename T>
inline Vector<COL, T> operator*(const Vector<ROW, T> &v, const Matrix<ROW, COL, T> &m) {
    Vector<COL, T> temp;
    for (size_t col = 0; col < COL; col++)
        temp[col] = Dot(v, m.GetCol(col));
    return temp;
}

//矩阵x列向量
template<size_t ROW, size_t COL, typename T>
inline Vector<ROW, T> operator*(const Matrix<ROW, COL, T> &m, const Vector<COL, T> &v) {
    Vector<ROW, T> temp;
    for (size_t row = 0; row < COL; row++)
        temp[row] = Dot(v, m.GetRow(row));
    return temp;
}

//一阶余子式
template<typename T>
inline T Cofactor(const Matrix<1, 1, T> &m, size_t row, size_t col) {
    return 0;
}

//多阶余子式：删除指定行列的子矩阵行列式
template<size_t N, typename T>
inline T Cofactor(const Matrix<N, N, T> &m, size_t row, size_t col) {
    return Det(m.GetMinor(row, col)) * (((row + col) % 2) ? -1 : 1);
}

//一阶行列式
template<typename T>
inline T Det(const Matrix<1, 1, T> &m) {
    return m[0][0];
}

//二阶行列式
template<typename T>
inline T Det(const Matrix<2, 2, T> &m) {
    return m[0][0] * m[1][1] - m[0][1] * m[1][0];
}

//多阶行列式
template<size_t N, typename T>
inline T Det(const Matrix<N, N, T> &m) {
    T sum = 0;
    for (size_t col = 0; col < N; col++)
        sum += m[0][col] * Cofactor(m, 0, col);
    return sum;
}

//伴随矩阵
template<size_t N, typename T>
inline Matrix<N, N, T> Adjoint(const Matrix<N, N, T> &m) {
    Matrix<N, N, T> ret;
    for (size_t row = 0; row < N; row++) {
        for (size_t col = 0; col < N; col++)
            ret[row][col] = Cofactor(m, row, col);
    }
    return ret;
}

//逆矩阵：伴随矩阵法
template<size_t N, typename T>
inline Matrix<N, N, T> Invert(const Matrix<N, N, T> &m) {
    Matrix<N, N, T> ret = Adjoint(m);
    T det = Dot(m.GetRow(0), ret.GetCol(0));
    return ret / det;
}

// 输出到文本流
template<size_t ROW, size_t COL, typename T>
inline std::ostream &operator<<(std::ostream &stream, const Matrix<ROW, COL, T> &m) {
    for (size_t row = 0; row < ROW; row++) {
        Vector<COL, T> v = m.Row(row);
        stream << v << std::endl;
    }
    return stream;
}


//矩阵别名

typedef Matrix<4, 4, float> Matrix4f;
typedef Matrix<3, 3, float> Matrix3f;