#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Vector.h"
#include "../Common/Object.h"


struct Model : Object {
    std::vector<Vector3f> positions;
    std::vector<Face3i> faces;
    std::vector<Vector2f> uvs;
    std::vector<Vector3f> normals;

    TGAImage diffuseMap;

    Model(const std::string &fileName);

    void LoadTexture(const std::string &fileName);
};