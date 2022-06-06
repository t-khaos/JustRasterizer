#pragma once

#include <cmath>
#include <algorithm>
#include "Global.h"

namespace Math{
    inline int ToRGB(float x) {
        return int(std::pow(std::clamp(x, 0.0f, 1.0f), 1.0f / 2.2f) * 255 + 0.5f);
    }

    inline float DegreeToRadian(float degrees) {
        return degrees * PI / 180.0;
    }

    inline std::tuple<float, float> Barycentric(const Vector<3, Vector<2, int>> &triangle, const Point2i &P) {
        auto &A = triangle[0];
        auto &B = triangle[1];
        auto &C = triangle[2];

        float numerator = static_cast<float>((A.y - B.y) * P.x + (B.x - A.x) * P.y + A.x * B.y - B.x * A.y);
        float denominator = static_cast<float>((A.y - B.y) * C.x + (B.x - A.x) * C.y + A.x * B.y - B.x * A.y);

        float gamma = numerator / denominator;

        numerator = static_cast<float>((A.y - C.y) * P.x + (C.x - A.x) * P.y + A.x * C.y - C.x * A.y);
        denominator = static_cast<float>((A.y - C.y) * B.x + (C.x - A.x) * B.y + A.x * C.y - C.x * A.y);

        float beta = numerator / denominator;

        return {beta, gamma};
    }
}