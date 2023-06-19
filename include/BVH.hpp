#pragma once

#include "raylib.h"
#include <vector>
#include <memory>
#include <algorithm>

struct AABB
{
    Vector3 Min;
    Vector3 Max;
};

struct BVHNode
{
    AABB Bounds;
    std::unique_ptr<BVHNode> Left;
    std::unique_ptr<BVHNode> Right;
    std::vector<int> triangleIndices;
};

class BVH
{
public:
    BVH(const unsigned short* indices);
    ~BVH();

private:

    std::unique_ptr<BVHNode> root;

    std::unique_ptr<BVHNode> ConstructBVH();
};

