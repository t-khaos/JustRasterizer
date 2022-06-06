
#include "Renderer.h"
#include "../Common/Vertex.h"

void Renderer::Render() const {

    Matrix4f M = Transform::Translate({0.0f, 0.0f, -3.0f})
                 * Transform::Scale({1.0f, 1.0f, 1.0f})
                 * Transform::Rotate({0.0f, 0.0f, 0.0f});

    Matrix4f V = Transform::Translate({0.0f, 0.0f, 0.0f})
                 * Transform::Rotate({0.0f, 0.0f, 0.0f})
                 * Transform::LookAt(camera->origin, camera->target, camera->up);

    Matrix4f P = Transform::Perspective(camera->aspectRatio, camera->fov, camera->near, camera->far);

    //渲染流水线
    //------------------------------------------------------------------------------------------------------------------------------------

    //遍历场景所有模型
    for (auto &model: scene->models) {
        //遍历模型所有三角面
        for (int j = 0; j < model->faces.size(); j++) {
            auto &face = model->faces[j];

            Vector<3, Vertex> triangle;
            Vector<3, Vector<2, int>> fragment;

            bool isClipped = false;

            //遍历所有顶点
            for (int i = 0; i < 3; i++) {
                auto &vertex = triangle[i];
                vertex.position = model->positions[face[i].x];
                vertex.uv = model->uvs[face[i].y];
                vertex.normal = model->normals[face[i].z];

                vertex.w = 1; //透视校正插值，保留w作为顶点深度值

                //设置MVP变换矩阵
                model->vShader->MVP = P * V * M;
                //设置法线变换矩阵：模型变换矩阵的伴随矩阵的转置
                model->vShader->N = Adjoint(M).Transpose();
                //设置光照
                model->fShader->lightDir = Vector3f(0, 0, 1);
                //设置摄像机位置
                model->vShader->cameraPos = camera->origin;


                //顶点着色
                //-----------------------------------------------
                model->vShader->VertexShading(vertex);
            }

            //齐次裁剪
            //-----------------------------------------------
            //背面剔除 Face Culling
            Vector3f viewDir(0, 0, 1); //NDC空间下观察方向为(0,0,1)
            auto &A = triangle[0].position;
            auto &B = triangle[1].position;
            auto &C = triangle[2].position;
            //模型三角面地顶点应遵循逆时针方向
            //在NDC空间的左手坐标系下，叉乘出的法向量与观察方向不同向则为背面
            auto faceNormal = Cross(B - A, C - A);
            if (Dot(viewDir, faceNormal) < 0)
                continue;


            for (int i = 0; i < 3; i++) {
                //视口剔除 CVV Culling
                //TODO: 在CVV上的三角形，顶点应重新规划。
                //简单剔除超出CVV的三角形
/*                float w = vertex.w;
                if (vertex.position.x > w || vertex.position.x < -w
                    || vertex.position.y > w || vertex.position.y < -w
                    || vertex.position.z > w || vertex.position.z < 0.0f
                    || w < near) {
                    isClipped = true;
                }*/

                //透视除法
                //-----------------------------------------------
                //为保证流水线的直观，故在这里做透视除法
                triangle[i].position /= triangle[i].w;

                //屏幕映射
                //-----------------------------------------------
                fragment[i] = Transform::Viewport(triangle[i].position, film->width, film->height);
            }

            //简单剔除超出CVV的三角形
            if (isClipped)
                continue;


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


                        model->fShader->normal = normal;
                        //此处应该换成纹理的尺寸
                        model->fShader->uv = Point2i(uv.x * film->width, uv.y * film->height);

                        //像素着色
                        //-----------------------------------------------
                        TGAColor color = model->fShader->FragmentShading();

                        //融合
                        //-----------------------------------------------

                        //深度测试
                        // 1/w与屏幕空间为线性关系，故可以用中心坐标进行插值
                        float wValue = alpha / triangle[0].w + beta / triangle[1].w + gamma / triangle[2].w;
                        wValue = 1.0f / wValue;
                        float zValue = triangle[0].position.z * alpha + triangle[1].position.z * beta + triangle[2].position.z * gamma;
                        zValue *= wValue;
                        int index = point.y * film->width + point.x;
                        if (film->zBuffer[index] > zValue) {
                            film->zBuffer[index] = zValue;
                            //绘制像素
                            film->image.set(point.x, point.y, color);
                        }
                    }
                }
            }
        }
    }

    film->Develop("output.tga");
}