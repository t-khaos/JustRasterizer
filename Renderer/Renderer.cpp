
#include "Renderer.h"
#include "../Common/Vertex.h"

void Renderer::Render() const {

    Matrix4f M = Transform::Translate({0.0f, 0.0f, -3.0f})
                 * Transform::Scale({1.0f, 1.0f, 1.0f})
                 * Transform::Rotate({0.0f, 0.0f, 0.0f});

    Matrix4f V = Transform::Translate({0.0f, 0.0f, 0.0f})
                 * Transform::Rotate({0.0f, 0.0f, 0.0f})
                 * Transform::LookAt(camera->origin, camera->target, camera->up);

    Matrix4f P = Transform::Perspective(camera->aspectRatio, camera->fov, 0.5f, 50.0f);

    //渲染流水线
    //------------------------------------------------------------------------------------------------------------------------------------

    //遍历场景所有模型
    for (auto &model: scene->models) {
        //遍历模型所有三角面
        for (int j = 0; j < model->faces.size(); j++) {
            auto &face = model->faces[j];

            Vector<3, Vertex> triangle;
            Vector<3, Vector<2, int>> fragment;

            //应用阶段
            //---------------------------------------------------------------------------------------
            //遍历所有顶点
            for (int i = 0; i < 3; i++) {
                //设置顶点数据
                auto &vertex = triangle[i];
                vertex.position = model->positions[face[i].x];
                vertex.uv = model->uvs[face[i].y];
                vertex.normal = model->normals[face[i].z];
                //透视校正插值，保留w1作为顶点深度值
                vertex.w1 = 1;
                vertex.w2 = 1;

                //设置变换矩阵
                model->vShader->MVP = P * V * M;
                //设置光照
                model->fShader->lightDir = Vector3f (0,0,1);

                //几何阶段
                //---------------------------------------------------------------------------------------
                //逐顶点应用顶点着色器

                //顶点着色
                //-----------------------------------------------
                model->vShader->VertexShading(vertex);

                //齐次裁剪
                //-----------------------------------------------


                //透视除法
                //-----------------------------------------------
                //为保证流水线的直观，故在这里做透视除法
                vertex.position /= vertex.w1;
                vertex.normal /= vertex.w2;

                //屏幕映射
                //-----------------------------------------------
                fragment[i] = Transform::Viewport(vertex.position, film->width, film->height);
            }

            //光栅化阶段
            //---------------------------------------------------------------------------------------
            //三角形设置
            //-----------------------------------------------
            Point2i minVector = MinVector(fragment[0], MinVector(fragment[1], fragment[2]));
            minVector = MaxVector(Point2i(0), minVector);
            Point2i maxVector = MaxVector(fragment[0], MaxVector(fragment[1], fragment[2]));
            maxVector = MinVector(Point2i(film->width - 1, film->height - 1), maxVector);

            //三角形遍历
            //-----------------------------------------------
            Point2i point;
            for (point.x = minVector.x; point.x < maxVector.x; point.x++) {
                for (point.y = minVector.y; point.y < maxVector.y; point.y++) {
                    auto [beta, gamma] = Math::Barycentric(fragment, point);

                    //beta,gamma可能算出0/0得NaN的情况
                    if (beta >= 0.0f && gamma >= 0.0f && (beta + gamma) <= 1.0f) {
                        float alpha = 1 - beta - gamma;

                        //法线插值
                        Vector3f normal = triangle[0].normal * alpha
                                          + triangle[1].normal * beta
                                          + triangle[2].normal * gamma;

                        //uv插值
                        Point2f uv = triangle[0].uv * alpha
                                     + triangle[1].uv * beta
                                     + triangle[2].uv * gamma;

                        Vertex vertex;
                        vertex.normal = normal;
                        vertex.uv = uv;
                        model->fShader->normal = normal;
                        //此处应该换成纹理的尺寸
                        model->fShader->uv = Point2i(uv.x * film->width, uv.y * film->height);

                        //像素着色
                        //-----------------------------------------------
                        TGAColor color = model->fShader->FragmentShading(vertex);

                        //透视校正插值
                        float wValue = alpha / triangle[0].w1 + beta / triangle[1].w1 + gamma / triangle[2].w1;
                        wValue = 1.0f / wValue;
                        float zValue = triangle[0].position.z * alpha + triangle[1].position.z * beta + triangle[2].position.z * gamma;
                        zValue *= wValue;

                        //写入Z-Buffer
                        int index = point.y * film->width + point.x;
                        if (film->zBuffer[index] > zValue) {
                            film->zBuffer[index] = zValue;
                            //绘制像素
                            film->image.set(point.x, point.y, color);
                        }
                    }
                }
            }
            //融合
            //-----------------------------------------------
        }
    }

    film->Develop("output.tga");
}