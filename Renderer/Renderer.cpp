
#include "Renderer.h"

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
            Vector<3, Vector<2, float>> fragment;

            //遍历所有顶点
            for (int i = 0; i < 3; i++) {
                auto &vertex = triangle[i];
                vertex.position = model->positions[face[i].x];
                vertex.uv = model->uvs[face[i].y];
                vertex.normal = model->normals[face[i].z];

                vertex.w = 1; //透视校正插值，保留w作为顶点深度值

                //设置MVP变换矩阵
                model->shader->MVP = P * V * M;
                //设置法线变换矩阵：模型变换矩阵的伴随矩阵的转置
                model->shader->N = Adjoint(M).Transpose();
                //设置光照
                model->shader->lightDir = Vector3f(0, 0, 1);
                //设置摄像机位置
                model->shader->cameraPos = camera->origin;


                //顶点着色
                //-----------------------------------------------
                model->shader->VertexShading(vertex);
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

            //视口剔除 CVV Culling
            for (int i = 0; i < 3; i++) {
                //TODO: 在CVV上的三角形，顶点应重新规划。
                //简单剔除超出CVV的三角形
/*                float w = vertex.w;
                if (vertex.position.x > w || vertex.position.x < -w
                    || vertex.position.y > w || vertex.position.y < -w
                    || vertex.position.z > w || vertex.position.z < 0.0f
                    || w < near) {

                }*/

                //透视除法
                //-----------------------------------------------
                //为保证流水线的直观，故在这里做透视除法
                triangle[i].position /= triangle[i].w;

                //屏幕映射
                //-----------------------------------------------
                fragment[i] = Transform::Viewport(triangle[i].position, film->width, film->height);
            }

            //光栅化阶段
            //---------------------------------------------------------------------------------------
            //三角形设置
            //-----------------------------------------------
            Point2f minVector = MinVector(fragment[0], MinVector(fragment[1], fragment[2]));
            minVector = MaxVector(Point2f(0.0f), minVector);
            Point2f maxVector = MaxVector(fragment[0], MaxVector(fragment[1], fragment[2]));
            maxVector = MinVector(Point2f(film->width, film->height), maxVector);


#define MSAA
            //三角形遍历
            //-----------------------------------------------
            Point2f point;
            //最大值向上取整，最小值向下取整且从中心点开始
            for (point.x = std::floor(minVector.x) + 0.5f; point.x < std::ceil(maxVector.x); point.x++) {
                for (point.y = std::floor(minVector.y) + 0.5f; point.y < std::ceil(maxVector.y); point.y++) {
                    Float3f ratios;
#ifdef MSAA
                    //MSAA
                    std::vector<float> matrix = {
                            1, 1,
                            1, 1
                    };
                    ConvKernel kernel(matrix);

                    Point2f subPoint;
                    for (auto &k: kernel.matrix) {
                        auto offset = kernel.Next();

                        offset.x = offset.x ? 1 : -1;
                        offset.y = offset.y ? 1 : -1;

                        subPoint = Point2f(point.x + offset.x * 0.25f, point.y + offset.y * 0.25f);
                        if (!Math::GetBarycentric(fragment, subPoint, ratios)) {
                            //未经过子像素，有效数量-1，k标记为0
                            kernel.valid--;
                            k = 0;
                        }
                    }

                    //子像素也没有经过则不算在三角形内
                    if (!kernel.valid)
                        continue;

                    //若有子像素经过，仍然需要以中心点计算重心坐标
                    Math::GetBarycentric(fragment, point, ratios);
#endif

#ifndef MSAA
                    //没有开启MSAA直接检查中心点是否在三角形内
                    if (!Math::GetBarycentric(fragment, point, ratios))
                        continue;
#endif

                    //法线插值
                    Vector3f normal = triangle[0].normal * ratios.x
                                      + triangle[1].normal * ratios.y
                                      + triangle[2].normal * ratios.z;

                    //uv插值
                    Point2f uv = triangle[0].uv * ratios.x
                                 + triangle[1].uv * ratios.y
                                 + triangle[2].uv * ratios.z;


                    model->shader->normal = normal;
                    //此处应该换成纹理的尺寸
                    model->shader->uv = uv;

                    //像素着色
                    //-----------------------------------------------
                    TGAColor pixel = model->shader->FragmentShading();

                    //融合
                    //-----------------------------------------------

                    //深度测试
                    // 1/w与屏幕空间为线性关系，故可以用中心坐标进行插值
                    float wValue = ratios.x / triangle[0].w + ratios.y / triangle[1].w + ratios.z / triangle[2].w;
                    wValue = 1.0f / wValue;
                    float depth = triangle[0].position.z * ratios.x + triangle[1].position.z * ratios.y + triangle[2].position.z * ratios.z;
                    depth *= wValue;
#ifdef MSAA
                    Color3i color;
                    kernel.Init();
                    bool update = false;
                    //遍历四个子像素
                    for (auto &k: kernel.matrix) {
                        auto offset = kernel.Next();
                        float x = point.x * 2 + offset.x;
                        float y = point.y * 2 + offset.y;

                        int index = y * film->width + x;

                        if (k && film->depthBufferMSAA2x2[index] > depth) {
                            film->depthBufferMSAA2x2[index] = depth;
                            film->frameBufferMSAA2x2[index] = pixel;
                            //子像素贡献颜色
                            color += Color3i(pixel.r, pixel.g, pixel.b);
                            update = true;
                        }
                        else{
                            TGAColor subPixel = film->frameBufferMSAA2x2[index];
                            color += Color3i(subPixel.r, subPixel.g, subPixel.b);
                        }
                    }
                    //子像素发生变动才需要更新
                    if(update){
                        //平均各个子像素的颜色
                        color /= kernel.Size();
                        pixel = TGAColor(color.r, color.g, color.b);
                        film->image.set(point.x, point.y, pixel);
                    }
#endif

#ifndef MSAA
                    int index = point.y * film->width + point.x;
                    if (film->depthBuffer[index] > depth) {
                        film->depthBuffer[index] = depth;
                        film->frameBuffer[index] = pixel;
                        film->image.set(point.x, point.y, pixel);
                    }
#endif
                }
            }
        }
    }

    film->Develop("output.tga");
}