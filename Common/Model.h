#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Vector.h"
#include "Shader.h"


struct Model {
    std::vector<Vector3f> positions;
    std::vector<Face3i> faces;
    std::vector<Vector2f> uvs;
    std::vector<Vector3f> normals;

    std::shared_ptr<Shader> shader;
};