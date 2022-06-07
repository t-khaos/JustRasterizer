cmake_minimum_required(VERSION 3.22)
project(JustRasterizer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


add_executable(
        JustRasterizer
        Application.cpp

        Model/Model.h
        Model/Vertex.h
        Model/Texture.h

        Include/JustMath/Vector.h
        Include/JustMath/Math.h
        Include/JustMath/Global.h
        Include/JustMath/Matrix.h
        Include/JustMath/Transform.h



        Renderer/Renderer.h Renderer/Renderer.cpp
        Renderer/Camera.h
        Renderer/Scene.h
        Renderer/Film.h Renderer/Film.cpp


        Include/TGA/tgaimage.h Include/TGA/tgaimage.cpp

        Shader/Shader.h
        Shader/DefaultShader.cpp
        Model/ConvKernel.h)
