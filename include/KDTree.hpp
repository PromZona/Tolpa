#pragma once

#include "raylib.h"
#include "raymath.h"
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
    Vector3 Pos;
    KDNode* Left;
    KDNode* Right;

    KDNode(Vector3 pos) : Left(nullptr), Right(nullptr) {this->Pos = pos;}
};

class KDTree
{
public:

    KDTree();
    ~KDTree();

    KDNode* ConstructTree(std::vector<Vector3>& nodePoints);

    inline KDNode** GetRoot(){return &m_root;}
    inline int& GetMaxDepth(){return m_maxDepth;}

    void DeleteTree(KDNode* node);
    void DebugDrawTree(KDNode* node, int depth, int targetDepth, bool inverted, bool elevated);
    void DebugDrawTreeByDepth(KDNode* root, int depth, bool inverted, bool elevated);

    bool IsBalanced(KDNode* root);
    bool ValidateKdTree(KDNode* root, int depth);

    // Find the node closest to the target pos
    Vector3 FindNearestNode(KDNode* root, Vector3 target, float &distance);

private:

    int m_maxDepth;
    std::vector<Vector3> m_nodePoints;
    KDNode* ConstructTree(std::vector<Vector3>& nodePoints, int depth);
    KDNode* m_root;
};