#pragma once

#include "../Include/JustMath/Vector.h"

struct EnvLight{
    Vector3f ambient;
};

struct DirectionLight{
    Vector3f direction;

    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
};

struct PointLight{
    Vector3f position;

    float constant;
    float linear;
    float quadratic;

    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;
};

struct SpotLight {
    Vector3f position;
    Vector3f direction;
    float cutOff;
    float outerCutOff;

    Vector3f ambient;
    Vector3f diffuse;
    Vector3f specular;

    float constant;
    float linear;
    float quadratic;
};