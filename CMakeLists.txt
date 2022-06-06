cmake_minimum_required(VERSION 3.22)
project(JustRenderer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


add_executable(
        JustRenderer
        Application.cpp

        Renderer/Film.h Renderer/Film.cpp

        Include/JustMath/Vector.h
        Include/JustMath/Math.h
        Include/JustMath/Global.h
        Include/JustMath/Matrix.h
        Include/JustMath/Transform.h

        Common/Model.h

        Renderer/Renderer.h
        Renderer/Camera.h
        Renderer/Scene.h

        Common/Vertex.h

        Include/TGA/tgaimage.h Include/TGA/tgaimage.cpp
        Common/Loader.h Shader/DefaultShader.cpp Common/Shader.h Renderer/Renderer.cpp)
