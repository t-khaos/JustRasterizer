#pragma once

#include <cmath>
#include <algorithm>
#include "Global.h"

namespace Math {
    inline int ToRGB(float x) {
        return int(std::pow(std::clamp(x, 0.0f, 1.0f), 1.0f / 2.2f) * 255 + 0.5f);
    }

    inline float DegreeToRadian(float degrees) {
        return degrees * PI / 180.0;
    }

    template<typename T>
    inline bool GetBarycentric(const Vector<3, Vector<2, T>> &triangle, const Vector<2, T> &P, Float3f &ratios) {
        auto &A = triangle[0];
        auto &B = triangle[1];
        auto &C = triangle[2];

        float numerator, denominator;

        numerator = static_cast<float>((A.y - C.y) * P.x + (C.x - A.x) * P.y + A.x * C.y - C.x * A.y);
        denominator = static_cast<float>((A.y - C.y) * B.x + (C.x - A.x) * B.y + A.x * C.y - C.x * A.y);
        ratios.y = numerator / denominator;

        numerator = static_cast<float>((A.y - B.y) * P.x + (B.x - A.x) * P.y + A.x * B.y - B.x * A.y);
        denominator = static_cast<float>((A.y - B.y) * C.x + (B.x - A.x) * C.y + A.x * B.y - B.x * A.y);
        ratios.z = numerator / denominator;

        ratios.x = 1.0f - ratios.y - ratios.z;

        return ratios.y >= 0 && ratios.z >= 0 && ratios.z + ratios.y <= 1.0f + EPSILON;
    }
}