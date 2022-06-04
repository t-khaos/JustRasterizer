#pragma once


#include <cmath>
#include <algorithm>
#include "../../JustTracer/Math/Global.h"
#include "Global.h"


inline int ToRGB(float x) {
    return int(std::pow(std::clamp(x, 0.0f, 1.0f), 1.0f / 2.2f) * 255 + 0.5f);
}

inline Point2i WorldToScreen(const Point3f &point) {
    return Point2i(int((point.x + 1.0f) * WIDTH / 2.0f + 0.5f), int((point.y + 1.0f) * HEIGHT / 2.0f + 0.5f));
}

inline float DegreeToRadian(float degrees) {
    return degrees * PI / 180.0;
}