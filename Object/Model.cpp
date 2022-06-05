#include "Model.h"

Model::Model(const std::string &fileName) {

    diffuseMap = TGAImage(1024, 1024, TGAImage::RGB);

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

void Model::LoadTexture(const std::string &fileName) {
    if (!diffuseMap.read_tga_file(fileName.c_str())){
        std::cerr << "Fail to load texture." << std::endl;
    }
    std::cout << "Succeed to load texture." << std::endl;

    diffuseMap.flip_vertically();
}