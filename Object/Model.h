#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "/Include/TGA/tgaimage.h"
#include "../Math/Vector.h"
#include "../Common/Object.h"

struct Model : Object {
    std::vector<Vector3f> positions;
    std::vector<Face3i> faces;
    std::vector<Vector2f> uvs;
    std::vector<Vector3f> normals;

    Model(const std::string &fileName) {
        std::ifstream fileStream(fileName);
        std::string line;

        while (std::getline(fileStream, line)) {
            std::istringstream strStream(line.c_str());
            char trash;
            if (!line.compare(0, 2, "v ")) {
                strStream >> trash;
                Vector3f position;
                for (int i = 0; i < 3; ++i) {
                    strStream >> position[i];
                }
                positions.push_back(position);
            } else if (!line.compare(0, 3, "vn ")) {
                strStream >> trash >> trash;
                Vector3f normal;
                for (int i = 0; i < 3; ++i)
                    strStream >> normal[i];
                normals.push_back(normal);
            } else if (!line.compare(0, 3, "vt ")) {
                strStream >> trash >> trash;
                Vector2f uv;
                for (int i = 0; i < 2; ++i)
                    strStream >> uv[i];
                uvs.push_back(uv);
            } else if (!line.compare(0, 2, "f ")) {
                Face3i face;
                Vector3i data;
                strStream >> trash;
                for (int i = 0; i < 3; ++i) {
                    strStream >> data[0] >> trash >> data[1] >> trash >> data[2];
                    for (int j = 0; j < 3; ++j)
                        face[i][j] = --data[j];//wavefront obj 文件索引从1开始
                }
                faces.push_back(face);
            }
        }
    }
};