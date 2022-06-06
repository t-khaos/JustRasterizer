
#include "Renderer/Film.h"
#include "Common/Model.h"
#include "Renderer/Scene.h"
#include "Common/Loader.h"
#include "Renderer/Renderer.h"

int main() {
    const int width = 1024;
    const int height = 1024;
    const float aspectRatio = static_cast<float>(width) / height;
    const float fov = 45;
    const float near = 0.5f, far = 50.0f;
    Vector3f origin(0.0f, 0.0f, 0.0f);
    Vector3f target(0.0f, 0.0f, -1.0f);
    Vector3f up(0.0f, 1.0f, 0.0f);

    //相机
    //-------------------------------------------
    auto camera = std::make_shared<Camera>(origin, target, up);
    camera->SetViewSpace(near, far, fov, aspectRatio);
    //胶卷
    //-------------------------------------------
    auto film = std::make_shared<Film>(width, height);

    //场景
    //-------------------------------------------
    auto scene = std::make_shared<Scene>();
    //模型
    auto model = Loader::LoadModel("../Resource/Model/african_head.obj");
    //着色器
    auto shader = std::make_shared<DefaultShader>();
    //纹理
    auto diffuseMap = Loader::LoadTexture("../Resource/Texture/african_head_diffuse.tga", film->width, film->height, TGAImage::RGB);

    shader->diffuseMap = diffuseMap;
    model->shader = shader;
    scene->AddModel(model);

    //渲染器
    //-------------------------------------------
    auto render = std::make_shared<Renderer>();
    render->camera = camera;
    render->film = film;
    render->scene = scene;

    render->Render();
}
