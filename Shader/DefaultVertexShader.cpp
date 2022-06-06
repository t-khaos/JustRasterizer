
#include "DefaultVertexShader.h"

void DefaultVertexShader::VertexShading(Vertex &vertex) {
    //MVP变换
    auto position4d = MVP * vertex.position.Get4D();
    vertex.w = position4d.w;
    vertex.position = position4d.Get3D();

    //法线变换
    auto normal4d = N * vertex.normal.Get4D();
    vertex.normal = Normalize(normal4d.Get3D());
}