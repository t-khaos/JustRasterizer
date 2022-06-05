#include <iostream>
#include "Renderer/Film.h"
#include "Object/Model.h"
#include "Include/JustMath/Math.h"
#include "Include/JustMath/Transform.h"
#include "Common/Vertex.h"

const Vector3f lightDir(0.0f, 0.0f, 1.0f);

std::tuple<float, float> Barycentric(const Vector<3, Vector<2, int>> &triangle, const Point2i &P) {
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

void DrawTriangleByBarycentric(Vector<3, Vertex> &t, std::vector<float> &zBuffer, Film &film, TGAImage &diffuseMap) {
    //视口变换
    Vector<3, Vector<2, int>> fragment;
    for (int i = 0; i < 3; i++)
        fragment[i] = WorldToScreen(t[i].position, film.width, film.height);

    //包围盒最小坐标
    Point2i minVector = MinVector(fragment[0], MinVector(fragment[1], fragment[2]));
    minVector = MaxVector(Point2i(0), minVector);
    //包围盒最大坐标
    Point2i maxVector = MaxVector(fragment[0], MaxVector(fragment[1], fragment[2]));
    maxVector = MinVector(Point2i(film.width - 1, film.height - 1), maxVector);

    Point2i P;
    for (P.x = minVector.x; P.x < maxVector.x; P.x++) {
        for (P.y = minVector.y; P.y < maxVector.y; P.y++) {
            auto [beta, gamma] = Barycentric(fragment, P);

            //beta,gamma可能算出0/0得NaN的情况
            if (beta >= 0.0f && gamma >= 0.0f && (beta + gamma) <= 1.0f) {
                float alpha = 1 - beta - gamma;

                //法线插值
                Vector3f normal = t[0].normal * alpha + t[1].normal * beta + t[2].normal * gamma;

                float NdotV = Dot(normal, lightDir);
                if (NdotV < 0)
                    return;

                //UV插值
                Point2f uv = t[0].uv * alpha + t[1].uv * beta + t[2].uv * gamma;
                TGAColor diffuseColor = diffuseMap.get(uv.x * film.width, uv.y * film.height);

                ///透视校正插值
                float wValue = alpha / t[0].w + beta / t[1].w + gamma / t[2].w;
                wValue = 1.0f / wValue;
                float zValue = t[0].position.z * alpha + t[1].position.z * beta + t[2].position.z * gamma;
                zValue *= wValue;

                int index = P.y * film.width + P.x;

                if (zBuffer[index] > zValue) {
                    zBuffer[index] = zValue;
                    film.image.set(P.x, P.y, TGAColor(diffuseColor.r * NdotV, diffuseColor.g * NdotV, diffuseColor.b * NdotV));
                }
            }
        }
    }
}


int main() {
    const int width = 1024;
    const int height = 1024;
    const float aspectRatio = static_cast<float>(width) / height;
    const float fov = 45;

    Film film(width, height);

    Model model("../Resource/Model/african_head.obj");
    model.LoadTexture("../Resource/Texture/african_head_diffuse.tga");

    std::vector<float> zBuffer(width * height, MAX_FLOAT);


    Matrix4f M = Translate({0.0f, 0.0f, -3.0f})
                 * Scale({1.0f, 1.0f, 1.0f})
                 * Rotate({0.0f, 60.0f, 0.0f});

    Vector3f origin(0.0f, 0.0f, 0.0f);
    Vector3f target(0.0f, 0.0f, -1.0f);
    Vector3f up(0.0f, 1.0f, 0.0f);
    Matrix4f V = Translate({0.0f, 0.0f, 0.0f})
                 * Rotate({0.0f, 0.0f, 0.0f})
                 * LookAt(origin, target, up);

    Matrix4f P = Perspective(aspectRatio, fov, 0.5f, 50.0f);

    for (int k = 0; k < model.faces.size(); k++) {
        auto &face = model.faces[k];

        Vector<3, Vertex> triangle;
        for (int j = 0; j < 3; j++) {

            auto &position = model.positions[face[j].x];
            auto &uv = model.uvs[face[j].y];
            auto &normal = model.normals[face[j].z];

            //MVP变换
            auto position4d = P * V * M * position.Get4D();

            auto normal4d = P * V * M * normal.Get4D();

            //齐次裁剪

            //透视校正插值，保留w作为深度值
            triangle[j].w = position4d.w;
            //透视除法
            triangle[j].position = position4d.Get3D() / position4d.w;
            triangle[j].uv = uv;
            triangle[j].normal = normal4d.Get3D() / normal4d.w;
        }
        DrawTriangleByBarycentric(triangle, zBuffer, film, model.diffuseMap);
    }
    film.Develop("output.tga");
}
