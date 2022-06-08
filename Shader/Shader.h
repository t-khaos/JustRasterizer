#pragma once

#include "../Include/JustMath/Vector.h"
#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Matrix.h"
#include "../Model/Vertex.h"
#include "../Model/Texture.h"

struct Shader{
    //common
    //------------------------------------------------------
    Point3f cameraPos;
    Point3f lightPos;

    //vertex shader
    //------------------------------------------------------
    Matrix4f MVP;
    Matrix4f N;
    Vertex vertex;

    virtual void VertexShading(Vertex &vertex) = 0;

    //fragment shader
    //------------------------------------------------------
    Vector3f normal;
    Point2f uv;
    Vector3f lightDir;
    std::shared_ptr<Texture> diffuseMap;
    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> specularMap;

    virtual TGAColor FragmentShading() = 0;
};

struct DefaultShader : Shader{
    virtual void VertexShading(Vertex &vertex) override;
    virtual TGAColor FragmentShading() override;
};