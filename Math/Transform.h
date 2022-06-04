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
            {1, 0,              0,               0},
            {0, cos(radians.x), -sin(radians.x), 0},
            {0, sin(radians.x), cos(radians.x),  0},
            {0, 0,              0,               1}
    };
    Matrix4f yRotation = {
            {cos(radians.y),  0, sin(radians.y), 0},
            {0,               1, 0,              0},
            {-sin(radians.y), 0, cos(radians.y), 0},
            {0,               0, 0,              1}
    };
    Matrix4f zRotation = {
            {cos(radians.z), -sin(radians.z), 0, 0},
            {sin(radians.z), cos(radians.z),  0, 0},
            {0,              0,               1, 0},
            {0,              0,               0, 1}
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
    //参数far和near为远近平面的长度，即正值
    float t = tan(DegreeToRadian(fov / 2)) * n;
    float r = t * aspectRatio;

    Matrix4f projection = {
            {n / r, 0,     0,                 0},
            {0,     n / t, 0,                 0},
            {0,     0,     -(n + f) / (f - n), -2 * f * n / (f - n)},
            {0,     0,     -1,                0}
    };


    return projection;
}

//获取摄像机朝向矩阵
inline Matrix4f LookAt(const Vector3f &origin, const Vector3f &target, const Vector3f &up) {
    Vector3f g = Normalize(target - origin);
    Vector3f gxt = Normalize(Cross(g, up));
    Vector3f t = Cross(gxt, g);

    Matrix4f translation = {
            {1, 0, 0, -origin.x},
            {0, 1, 0, -origin.y},
            {0, 0, 1, -origin.z},
            {0, 0, 0, 1}
    };

    Matrix4f rotation = {
            {gxt.x, gxt.y, gxt.z, 0},
            {t.x,   t.y,   t.z,   0},
            {-g.x,  -g.y,  -g.z,  0},
            {0,     0,     0,     1}
    };

    return rotation * translation;
}