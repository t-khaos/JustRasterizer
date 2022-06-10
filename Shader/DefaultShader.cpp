
#include "Shader.h"

void DefaultShader::VertexShading(Vertex &vertex) {
    //MVP变换
    auto position4d = ProjectionMatrix * ViewMatrix * ModelMatrix * vertex.position.Get4D();
    vertex.w = position4d.w;
    vertex.position = position4d.Get3D();

    //顶点的世界空间坐标
    auto worldPos4d = ModelMatrix * vertex.position.Get4D();
    vertex.worldPos = worldPos4d.Get3D();

    //法线变换
    auto normal4d = NormalMatrix * vertex.normal.Get4D();
    vertex.normal = Normalize(normal4d.Get3D());
}

TGAColor DefaultShader::FragmentShading() {
    float NdotL = Dot(fragment.normal, pointLight.position - fragment.position);
    if (NdotL < 0)
        return TGAColor(0, 0, 0);
    //TGAColor color = diffuseMap->GetColor(uv, 3);
    TGAColor color = diffuseMap->GetColorBilinear(fragment.uv, 3);
    return TGAColor(color.r * NdotL, color.g * NdotL, color.b * NdotL);
}