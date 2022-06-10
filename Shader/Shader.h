#pragma once

#include "../Include/JustMath/Vector.h"
#include "../Include/TGA/tgaimage.h"
#include "../Include/JustMath/Matrix.h"
#include "../Model/Vertex.h"
#include "../Model/Texture.h"
#include "Light.h"

struct Shader{
    //common
    //------------------------------------------------------
    Point3f cameraPos;
    DirectionLight dirLight;
    PointLight pointLight;
    EnvLight envLight;

    //vertex shader
    //------------------------------------------------------
    Matrix4f ModelMatrix,ViewMatrix,ProjectionMatrix;
    Matrix4f NormalMatrix;
    Vertex vertex;

    virtual void VertexShading(Vertex &vertex) = 0;

    //fragment shader
    //------------------------------------------------------
    Vertex fragment;
    std::shared_ptr<Texture> diffuseMap;
    std::shared_ptr<Texture> normalMap;
    std::shared_ptr<Texture> specularMap;

    virtual TGAColor FragmentShading() = 0;
};

struct DefaultShader : Shader{
    virtual void VertexShading(Vertex &vertex) override;
    virtual TGAColor FragmentShading() override;
};

struct BlinnPhongShader : Shader{
    Color3f objectColor;

    virtual void VertexShading(Vertex &vertex) override;
    virtual TGAColor FragmentShading() override;

    Color3f CalcDirectionLight(const DirectionLight &dirLight, const Vector3f &normal, const Vector3f &fragPos, const Vector3f &viewDir);
    Color3f CalcPointLight(const PointLight &pointLight, const Vector3f &N, const Vector3f &fragPos, const Vector3f &V);
};
