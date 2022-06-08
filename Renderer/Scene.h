#pragma once

#include <memory>
#include <vector>

#include "../Model/Model.h"

struct Scene {
    std::vector<std::shared_ptr<Model>> models;

    Scene():models({}){}

    void AddModel(const std::shared_ptr<Model> &model) { models.push_back(model); }
};