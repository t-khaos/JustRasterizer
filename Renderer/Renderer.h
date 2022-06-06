#pragma once

#include <memory>

#include "../Include/JustMath/Matrix.h"
#include "../Include/JustMath/Transform.h"
#include "Film.h"
#include "Camera.h"
#include "Scene.h"


struct Renderer {

    std::shared_ptr<Film> film;
    std::shared_ptr<Camera> camera;
    std::shared_ptr<Scene> scene;

    Renderer() = default;

    void Render() const;
};
