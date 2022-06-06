#pragma once

#include <memory>
#include <vector>
#include "../Common/Model.h"

struct Scene {
    std::vector<std::shared_ptr<Model>> models;

    Scene():models({}){}

    void AddModel(std::shared_ptr<Model> model) { models.push_back(model); }
};