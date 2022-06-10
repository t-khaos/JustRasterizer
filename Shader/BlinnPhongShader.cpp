
#include "Shader.h"


void BlinnPhongShader::VertexShading(Vertex &vertex) {
    //模型变换变换
    auto position4d = ModelMatrix * vertex.position.Get4D();
    //记录顶点的世界空间坐标
    vertex.worldPos = position4d.Get3D();
    //视图变换
    position4d = ViewMatrix * position4d;
    //透视投影变换
    position4d = ProjectionMatrix * position4d;
    vertex.w = position4d.w;
    vertex.position = position4d.Get3D();

    //将法线变换到世界空间
    auto normal4d = NormalMatrix * vertex.normal.Get4D();
    vertex.normal = Normalize(normal4d.Get3D());
}

TGAColor BlinnPhongShader::FragmentShading() {
    //采样纹理
    TGAColor tgaColor = diffuseMap->GetColorBilinear(fragment.uv, 0);
    objectColor = Color3f(tgaColor.r, tgaColor.g, tgaColor.b);

    //在世界空间计算光照
    Vector3f N = Normalize(fragment.normal);
    Vector3f V = Normalize(cameraPos - fragment.worldPos);


    Color3f result = CalcDirectionLight(dirLight, N, -dirLight.direction, V)
                     + CalcPointLight(pointLight, N, Normalize(pointLight.position - fragment.worldPos), V);

    result = ClampVector(result, 0.0f, 255.0f);

    return TGAColor(result.r, result.g, result.b);
}

Color3f BlinnPhongShader::CalcPointLight(const PointLight &pointLight, const Vector3f &N, const Vector3f &L, const Vector3f &V) {

    Vector3f H = Normalize(V + L);
    float NdotL = std::max(Dot(N, L), 0.0f);
    float NdotH = std::max(Dot(N, H), 0.0f);

    float distance = Length(L);
    float attenuation = 1.0f / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    float specular = std::pow(NdotH, 32);

    return (pointLight.ambient + pointLight.diffuse * NdotL + pointLight.specular * specular) * attenuation * objectColor;
}

Color3f BlinnPhongShader::CalcDirectionLight(const DirectionLight &dirLight, const Vector3f &N, const Vector3f &L, const Vector3f &V) {
    Vector3f H = Normalize(V + L);
    float NdotL = std::max(Dot(N, L), 0.0f);
    float NdotH = std::max(Dot(N, H), 0.0f);

    float specular = std::pow(NdotH, 8);

    return (dirLight.ambient + dirLight.diffuse * NdotL + dirLight.specular * specular) * objectColor;
}
