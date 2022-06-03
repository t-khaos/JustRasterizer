#pragma once

struct Shader{
    virtual void VertexShader()=0;
    virtual void FragmentShader()=0;
};