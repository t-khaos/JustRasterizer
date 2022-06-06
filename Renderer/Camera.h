#pragma once

#include "../Include/JustMath/Vector.h"

struct Camera {
    Point3f origin;
    Point3f target;
    Vector3f up;
    float fov;
    float aspectRatio;
    float near, far;

    Camera(const Point3f &_ori, const Point3f &_tar, const Vector3f _up)
            : origin(_ori), target(_tar), up(_up) {}

    void SetViewSpace(float _near, float _far, float _fov, float _ratio) {
        near = _near;
        far = _far;
        fov = _fov;
        aspectRatio = _ratio;
    }
};