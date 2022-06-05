
#include "DefaultVertexShader.h"

Vertex DefaultVertexShader::VertexShading(Vertex &vertex) {
    //MVP变换
    vertex.position = MVP * vertex.position;
    vertex.normal = MVP * vertex.normal;

    return vertex;
}