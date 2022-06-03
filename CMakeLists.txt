cmake_minimum_required(VERSION 3.22)
project(JustRenderer)

set(CMAKE_CXX_STANDARD 17)

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")


add_executable(
        JustRenderer
        Application.cpp
        Math/Vector.h
        Renderer/Film.h Renderer/Film.cpp
        Math/Math.h
        Math/Global.h Object/Model.h Math/Matrix.h Math/Transform.h Renderer/Renderer.h Renderer/Camera.h Renderer/Scene.h Common/Object.h Common/Shader.h Common/Texture.h)
