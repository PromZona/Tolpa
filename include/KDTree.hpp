#pragma once

#include "raylib.h"
#include <vector>
#include <cmath>
#include <algorithm>

inline bool operator==(const Vector3& v1, const Vector3& v2) {
    return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
};

inline bool operator!=(const Vector3& v1, const Vector3& v2) {
    return (v1.x != v2.x) || (v1.y != v2.y) || (v1.z != v2.z);
};

struct KDNode
{
    int SplitAxis;
    Vector3 pos;
    KDNode* left;
    KDNode* right;

    KDNode(Vector3 pos) : left(nullptr), right(nullptr) {this->pos = pos;}
};

class KDTree
{
public:

    KDTree();
    ~KDTree();

    KDNode* constructTree(std::vector<Vector3>& nodePoints);

    inline KDNode** GetRoot(){return &root;}
    inline int& GetMaxDepth(){return m_maxDepth;}

    void DeleteTree(KDNode* node);
    void DebugDrawTree(KDNode* node, int depth, int targetDepth, bool inverted, bool elevated);
    void DebugDrawTreeByDepth(KDNode* root, int depth, bool inverted, bool elevated);

    bool isBalanced(KDNode* root);
    bool validateKdTree(KDNode* root, int depth);

    // Find the node closest to the target pos
    Vector3 findNearestNode(KDNode* root, Vector3 target, float &distance);

private:

    int m_maxDepth;
    std::vector<Vector3> nodePoints;
    KDNode* constructTree(std::vector<Vector3>& nodePoints, int depth);
    KDNode* root;
};