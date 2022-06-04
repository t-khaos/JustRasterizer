#pragma once

#include "Matrix.h"
#include "Vector.h"


//获取旋转矩阵
inline Matrix4f Rotate(const Vector3f &angles) {
    Vector3f radians(
            DegreeToRadian(angles.x),
            DegreeToRadian(angles.y),
            DegreeToRadian(angles.z)
    );

    Matrix4f xRotation = {
            {1, 0,                   0,                    0},
            {0, cos(radians.x), -sin(radians.x), 0},
            {0, sin(radians.x), cos(radians.x),  0},
            {0, 0,                   0,                    1}
    };
    Matrix4f yRotation = {
            {cos(radians.y),  0, sin(radians.y), 0},
            {0,                    1, 0,                   0},
            {-sin(radians.y), 0, cos(radians.y), 0},
            {0,                    0, 0,                   1}
    };
    Matrix4f zRotation = {
            {cos(radians.z), -sin(radians.z), 0, 0},
            {sin(radians.z), cos(radians.z),  0, 0},
            {0,                   0,                    1, 0},
            {0,                   0,                    0, 1}
    };

    return zRotation * yRotation * xRotation;
}

//获取平移矩阵
inline Matrix4f Translate(const Vector3f &move) {
    return {
            {1, 0, 0, move.x},
            {0, 1, 0, move.y},
            {0, 0, 1, move.z},
            {0, 0, 0, 1}
    };
}

//获取缩放矩阵
inline Matrix4f Scale(const Vector3f &ratios) {
    return {
            {ratios.x, 0,        0,        0},
            {0,        ratios.y, 0,        0},
            {0,        0,        ratios.z, 0},
            {0,        0,        0,        1}
    };
}

//获取透视投影矩阵
inline Matrix4f Perspective(float aspectRatio, float fov, float n, float f) {
    float t = tan(DegreeToRadian(fov / 2));
    float b = -t;
    float r = t / aspectRatio;
    float l = -r;

    Matrix4f perspective = {
            {n, 0, 0,     0},
            {0, n, 0,     0},
            {0, 0, n + f, -n * f},
            {0, 0, 1,     0}
    };

    Matrix4f translation = {
            {1, 0, 0, 0},
            {0, 1, 0, 0},
            {0, 0, 1, -(n + f) / 2},
            {0, 0, 0, 1}
    };

    Matrix4f scale = {
            {1 / r, 0,     0,           0},
            {0,     1 / t, 0,           0},
            {0,     0,     2 / (n - f), 0},
            {0,     0,     0,           1}
    };

    Matrix4f projection = scale * translation * perspective;

    return projection;
}

//获取摄像机朝向矩阵
inline Matrix4f LookAt(const Vector3f &origin, const Vector3f &target, const Vector3f &up) {
    Vector3f w = Normalize(origin - target);
    Vector3f u = Normalize(Cross(up, w));
    Vector3f v = Cross(w, u);

    return {
            {w.x, w.y, w.z, -Dot(origin, w)},
            {u.x, u.y, u.z, -Dot(origin, u)},
            {v.x, v.y, v.z, -Dot(origin, v)},
            {0,   0,   0,   1}
    };
}