#include <iostream>
#include "Renderer/Film.h"
#include "Math/Global.h"
#include "Object/Model.h"
#include "Math/Math.h"
#include "Math/Transform.h"

const Vector3f lightDir(0.0f, 0.0f, -1.0f);

//Bresenham画线算法
void DrawLine(Point2i &p0, Point2i &p1, std::vector<Color3f> &pixels, const Color3f &color) {
    //斜率
    bool flag = false;
    if (std::abs(p0.x - p1.x) < std::abs(p0.y - p1.y)) {
        SwapValue(p0);
        SwapValue(p1);
        flag = true;
    }
    //顺序
    if (p0.x > p1.x)
        SwapVector(p0, p1);

    for (int x = p0.x; x <= p1.x; x++) {
        float t = (x - p0.x) / (float) (p1.x - p0.x);
        int y = p0.y * (1.0f - t) + p1.y * t;
        if (flag)
            pixels[x * WIDTH + y] = color;
        else
            pixels[y * WIDTH + x] = color;
    }
    std::cout << "Draw Done." << std::endl;
}

std::tuple<float, float> Barycentric(const Triangle2i &triangle, const Point2i &P) {
    auto &A = triangle[0];
    auto &B = triangle[1];
    auto &C = triangle[2];

    float gamma = static_cast<float >((A.y - B.y) * P.x + (B.x - A.x) * P.y + A.x * B.y - B.x * A.y)
                  / ((A.y - B.y) * C.x + (B.x - A.x) * C.y + A.x * B.y - B.x * A.y);

    float beta = static_cast<float >((A.y - C.y) * P.x + (C.x - A.x) * P.y + A.x * C.y - C.x * A.y)
                 / ((A.y - C.y) * B.x + (C.x - A.x) * B.y + A.x * C.y - C.x * A.y);

    return {beta, gamma};
}


void DrawTriangleByBarycentric(Triangle3f &triangle, std::vector<float> &zBuffer, std::vector<Color3f> &pixels, const Color3f &color) {
    //转为2D
    Triangle2i fragment;
    for (int i = 0; i < 3; i++)
        fragment[i] = WorldToScreen(triangle[i]);

    //包围盒最小坐标
    Point2i minVector = MinVector(fragment[0], MinVector(fragment[1], fragment[2]));
    minVector = MaxVector(Point2i(0), minVector);
    //包围盒最大坐标
    Point2i maxVector = MaxVector(fragment[0], MaxVector(fragment[1], fragment[2]));
    maxVector = MinVector(Point2i(WIDTH - 1, HEIGHT - 1), maxVector);

    Point2i P;
    for (P.x = minVector.x; P.x < maxVector.x; P.x++) {
        for (P.y = minVector.y; P.y < maxVector.y; P.y++) {
            auto [beta, gamma] = Barycentric(fragment, P);
            if (beta < 0.0f || gamma < 0.0f || (beta + gamma) > 1.0f)
                continue;

            //插值z
            float zValue = triangle[0].z * (1.0f - beta - gamma) + triangle[1].z * beta + triangle[2].z * gamma;

            int index = P.y * WIDTH + P.x;

            if (zBuffer[index] <= zValue) {
                zBuffer[index] = zValue;
                pixels[index] = color;
            }
        }
    }
}


int main() {

    Model model("../Resource/Model/african_head.obj");


    Film film(WIDTH, HEIGHT);
    std::vector<Color3f> pixels(WIDTH * HEIGHT, Color3f(0.0f));

    const Color3f WHITE(1.0f);
    const Color3f RED(1.0f, 0.0f, 0.0f);

    std::vector<float> zBuffer(WIDTH * HEIGHT, -MAX_FLOAT);


    Matrix4f rotateModel = Rotate({0.0f, -60.0f, 0.0f});
    Matrix4f scaleModel = Scale({0.9f, 0.9f, 0.9f});
    Matrix4f translateModel = Translate({0.0f, 0.0f, 0.0f});
    Matrix4f M = translateModel * scaleModel * rotateModel;


    for (int k = 0; k < model.faces.size(); k++) {
        auto &face = model.faces[k];

        Point2i screenCoords[3];
        Triangle3f triangle;
        for (int j = 0; j < 3; j++) {
            auto &v = model.positions[face[j].x];

            auto position = M * v.Get4D();
            screenCoords[j] = WorldToScreen(position.Get3D());
            triangle[j] = position.Get3D();
        }
        Vector3f normal = Cross(
                triangle[2] - triangle[0],
                triangle[1] - triangle[0]
        );

        normal = Normalize(normal);

        float NdotV = Dot(normal, lightDir);

        if (NdotV > 0) {
            //DrawTriangleByLine(screenCoords[0], screenCoords[1], screenCoords[2], pixels, WHITE * NdotV);
            DrawTriangleByBarycentric(triangle, zBuffer, pixels, WHITE * NdotV);
        }

    }

    film.Develop(pixels);

}
