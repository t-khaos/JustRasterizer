#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Vector.h"
#include "../Common/VertexShader.h"
#include "../Common/FragmenShader.h"


struct Model {
    std::vector<Vector3f> positions;
    std::vector<Face3i> faces;
    std::vector<Vector2f> uvs;
    std::vector<Vector3f> normals;

    std::shared_ptr<VertexShader> vShader;
    std::shared_ptr<FragmentShader> fShader;
};