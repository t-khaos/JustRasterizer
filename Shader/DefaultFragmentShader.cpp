
#include "DefaultFragmentShader.h"

TGAColor DefaultFragmentShader::FragmentShading(Vertex &vertex){
    float NdotL = Dot(normal, lightDir);
    if (NdotL < 0)
        return TGAColor(0, 0, 0);
    TGAColor color = diffuseMap->get(uv.x, uv.y);
    return TGAColor(color.r * NdotL, color.g * NdotL, color.b * NdotL);
}