
#include "DefaultVertexShader.h"

void DefaultVertexShader::VertexShading(Vertex &vertex) {
    //MVP变换
    auto position4d = MVP * vertex.position.Get4D();
    vertex.w1 = position4d.w;
    vertex.position = position4d.Get3D();

    auto normal4d = MVP * vertex.normal.Get4D();
    vertex.w2 = normal4d.w;
    vertex.normal = normal4d.Get3D();
}