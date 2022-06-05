#pragma once

#include <memory>

#include "../Include/JustMath/Matrix.h"
#include "../Include/JustMath/Transform.h"
#include "Film.h"
#include "Camera.h"
#include "Scene.h"


struct Renderer {

    std::shared_ptr<Film> film;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Scene> scene;

    Renderer() = default;

    void Render() const;

    void Rasterize(const Vector<3, Vertex> &triangle, const Vector<3, Vector<2, int>> &fragment, std::shared_ptr<FragmentShader> fShader) const;
};

void Renderer::Render() const {

    //此处应改为应用阶段设置
    Matrix4f M = Transform::Translate({0.0f, 0.0f, -3.0f})
                 * Transform::Scale({1.0f, 1.0f, 1.0f})
                 * Transform::Rotate({0.0f, 0.0f, 0.0f});

    Matrix4f V = Transform::Translate({0.0f, 0.0f, 0.0f})
                 * Transform::Rotate({0.0f, 0.0f, 0.0f})
                 * Transform::LookAt(camera->origin, camera->target, camera->up);

    Matrix4f P = Transform::Perspective(camera->aspectRatio, camera->fov, 0.5f, 50.0f);

    //遍历场景所有模型
    for (auto &model: scene->models) {
        //遍历模型所有三角面
        for (int j = 0; j < model->faces.size(); j++) {
            auto &face = model->faces[j];

            Vector<3, Vertex> triangle;
            Vector<3, Vector<2, int>> fragment;

            //几何阶段
            //-------------------------------------------------------------------------
            //逐顶点应用顶点着色器
            for (int i = 0; i < 3; i++) {

                auto &vertex = triangle[i];

                vertex.position = model->positions[face[i].x].Get4D();
                vertex.uv = model->uvs[face[i].y];
                vertex.normal = model->normals[face[i].z].Get4D();
                vertex.w = 1;

                model->vShader->MVP = P * V * M;
                model->vShader->VertexShading(vertex);

                //齐次裁剪

                //透视校正插值，保留w作为深度值
                vertex.w = vertex.position.w;
                //透视除法
                vertex.position /= vertex.position.w;
                vertex.normal /= vertex.normal.w;

            }

            //光栅化阶段
            //-------------------------------------------------------------------------
            //视口变换
            for (int i = 0; i < 3; i++)
                fragment[i] = Transform::Viewport(triangle[i], film->width, film->height);
            //逐三角形应用片元着色器
            Rasterize(triangle, fragment, model->fShader);
        }
    }

    film->Develop("output.tga");
}

void Renderer::Rasterize(const Vector<3, Vertex> &triangle, const Vector<3, Vector<2, int>> &fragment, std::shared_ptr<FragmentShader> fShader) const {

    //包围盒最小坐标
    Point2i minVector = MinVector(fragment[0], MinVector(fragment[1], fragment[2]));
    minVector = MaxVector(Point2i(0), minVector);
    //包围盒最大坐标
    Point2i maxVector = MaxVector(fragment[0], MaxVector(fragment[1], fragment[2]));
    maxVector = MinVector(Point2i(film->width - 1, film->height - 1), maxVector);

    Point2i P;
    for (P.x = minVector.x; P.x < maxVector.x; P.x++) {
        for (P.y = minVector.y; P.y < maxVector.y; P.y++) {
            auto [beta, gamma] = Math::Barycentric(fragment, P);

            //beta,gamma可能算出0/0得NaN的情况
            if (beta >= 0.0f && gamma >= 0.0f && (beta + gamma) <= 1.0f) {
                float alpha = 1 - beta - gamma;

                //法线插值
                Vector3f normal = triangle[0].normal.Get3D() * alpha
                                  + triangle[1].normal.Get3D() * beta
                                  + triangle[2].normal.Get3D() * gamma;

                //UV插值
                Point2f uv = triangle[0].uv * alpha
                             + triangle[1].uv * beta
                             + triangle[2].uv * gamma;

                fShader->normal = normal;
                //此处应该换成纹理的尺寸
                fShader->uv = Point2i(uv.x * film->width, uv.y * film->height);

                TGAColor color = fShader->FragmentShading();

                ///透视校正插值
                float wValue = alpha / triangle[0].w + beta / triangle[1].w + gamma / triangle[2].w;
                wValue = 1.0f / wValue;
                float zValue = triangle[0].position.z * alpha + triangle[1].position.z * beta + triangle[2].position.z * gamma;
                zValue *= wValue;

                int index = P.y * film->width + P.x;

                if (film->zBuffer[index] > zValue) {
                    film->zBuffer[index] = zValue;
                    film->image.set(P.x, P.y, color);
                }
            }
        }
    }
}
