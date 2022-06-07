
#include "Shader.h"

void DefaultShader::VertexShading(Vertex &vertex) {
    //MVP变换
    auto position4d = MVP * vertex.position.Get4D();
    vertex.w = position4d.w;
    vertex.position = position4d.Get3D();

    //法线变换
    auto normal4d = N * vertex.normal.Get4D();
    vertex.normal = Normalize(normal4d.Get3D());
}

TGAColor DefaultShader::FragmentShading() {
    float NdotL = Dot(normal, lightDir);
    if (NdotL < 0)
        return TGAColor(0, 0, 0);
    TGAColor color = diffuseMap->GetColorByUV(uv, 3);
    return TGAColor(color.r * NdotL, color.g * NdotL, color.b * NdotL);
}