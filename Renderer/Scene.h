#pragma once;

#include <memory>
#include <vector>
#include "../Common/Object.h"

struct Scene{
    std::vector<std::shared_ptr<Object>> objects;
    //透明物体
    std::vector<std::shared_ptr<Object>> transparentObjects;

    void AddModel();
};