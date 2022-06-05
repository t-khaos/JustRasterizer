#pragma once

#include "../Include/JustMath/Vector.h"

struct Camera {
    Point3f origin;
    Point3f target;
    Vector3f up;
    float fov;
    float aspectRatio;

    Camera(const Point3f &_ori, const Point3f &_tar,
           const Vector3f _up,
           float _fov, float _ratio)
            : origin(_ori), target(_tar),
              up(_up), fov(_fov), aspectRatio(_ratio) {}
};