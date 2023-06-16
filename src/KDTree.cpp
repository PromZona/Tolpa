#include "KDTree.hpp"

KDTree::KDTree()
{
    m_maxDepth = 0;
}
KDTree::~KDTree()
{
    DeleteTree(root);
}

void KDTree::DeleteTree(KDNode* node)
{
    if (node == nullptr)
        return;

    DeleteTree(node->left);
    DeleteTree(node->right);

    delete(node);
}

float GetDistance(Vector3 v1, Vector3 v2)
{
    float dx = v1.x - v2.x;
    float dy = v1.y - v2.y;
    float dz = v1.z - v2.z;

    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

int calculateSize(KDNode* root) 
{
    if (root == nullptr)
        return 0;

    return 1 + calculateSize(root->left) + calculateSize(root->right);
}

bool ComparePointsInDimensions(Vector3 p1, Vector3 p2, int axis)
{
    switch (axis)
    {
        case 0:
            return p1.x < p2.x;

        case 1:
            return p1.y < p2.y;

        case 2:
            return p1.z < p2.z;
    }

    TraceLog(LOG_WARNING, "EQUAL POINTS IN MESH???? AAAAAAAAAAHHHHHH");
    return false; // Points are considered equal (Apocalypse happens)
}

// This ensures that right side will always have strictly higher points
// And left lesser or equal, im not sure if this is useful in any way
// but it appeals to the common design of the k-d tree
// also makes tree disbalanced but not too much to have serious effects
// also kind of makes data more predictable so thats it i guess
int SelectLastEqualPoint(std::vector<Vector3>& nodePoints, int axis, int index)
{
    if (index + 1 == nodePoints.size())
        return index;

    switch(axis)
    {
        case 0:
            while (nodePoints[index].x == nodePoints[index + 1].x)
            {
                if (index + 1 == nodePoints.size())
                    return index;
                index++;
            }

            break;
        case 1:
            while (nodePoints[index].y == nodePoints[index + 1].y)
            {
                 if (index + 1 == nodePoints.size())
                    return index;
                index++;
            }

            break;
        case 2:
            while (nodePoints[index].z == nodePoints[index + 1].z)
            {
                if (index + 1 == nodePoints.size())
                    return index;
                index++;
            }

            break;
    }

    return index;
}

int FindMedianIndex(std::vector<Vector3>& nodePoints, int axis) 
{
    std::sort(nodePoints.begin(), nodePoints.end(), [axis](const Vector3 v1, const Vector3 v2) {
        return ComparePointsInDimensions(v1, v2, axis);
    });

    int medianPointIndex = nodePoints.size() / 2;

    medianPointIndex = SelectLastEqualPoint(nodePoints, axis, medianPointIndex);

    return medianPointIndex;
}

// this is somewhat unnecessary?? (might find some use later though)
// it leaves the original set of points as it is, unsorted
// and allows to store tree points inside tree class (?????)
KDNode* KDTree::constructTree(std::vector<Vector3>& nodePoints)
{
    this->nodePoints = nodePoints;
    return constructTree(this->nodePoints, 0);
}

KDNode* KDTree::constructTree(std::vector<Vector3>& nodePoints, int depth) 
{
    if (nodePoints.empty())
        return nullptr;

    int axis = depth % 3;

    int medianIndex = FindMedianIndex(nodePoints, axis);
    KDNode* root = new KDNode(nodePoints[medianIndex]);
    root->SplitAxis = axis;

    std::vector<Vector3> leftPoints, rightPoints;

    leftPoints = std::vector<Vector3>(nodePoints.begin(), nodePoints.begin() + medianIndex);
    rightPoints = std::vector<Vector3>(nodePoints.begin() + medianIndex + 1, nodePoints.end());

    root->left = constructTree(leftPoints, depth + 1);
    root->right = constructTree(rightPoints, depth + 1);

    if (m_maxDepth < depth)
        m_maxDepth = depth;

    return root;
}

Vector3 KDTree::findNearestNode(KDNode* root, Vector3 target, float& distance)
{
    static Vector3 s_nearest;

    if (root == nullptr)
        return s_nearest;

    DrawLine3D(target, root->pos, RAYWHITE);
    if (Vector3Distance(root->pos, target) <= distance)
    {
        distance = Vector3Distance(root->pos, target);
        s_nearest = root->pos;
    }

    switch(root->SplitAxis)
    {
        case 0:
            if (target.x <= root->pos.x || root->right == nullptr)
                s_nearest = findNearestNode(root->left, target, distance);
            else
                s_nearest = findNearestNode(root->right, target, distance);

            break;
        case 1:
            if (target.y <= root->pos.y || root->right == nullptr)
                s_nearest = findNearestNode(root->left, target, distance);
            else
                s_nearest = findNearestNode(root->right, target, distance);
                
            break;
        case 2:
            if (target.z <= root->pos.z || root->right == nullptr)
                s_nearest = findNearestNode(root->left, target, distance);
            else
                s_nearest = findNearestNode(root->right, target, distance);
                
            break;
    }

    DrawLine3D(target, s_nearest, MAGENTA);
    return s_nearest;
}

#include "raymath.h"
void KDTree::DebugDrawTree(KDNode* node, int depth, int targetDepth, bool inverted, bool elevated)
{
    if (node == nullptr)
        return;

    if (node->left != nullptr)
    {
        if (elevated)
            DrawLine3D(Vector3Add(node->pos, {0.0f, 20.0f * m_maxDepth / (depth + 1) - 20, 0.0f}), 
                       Vector3Add(node->left->pos, {0.0f, 20.0f * m_maxDepth / (depth + 2) - 20}), GREEN);
        else
            DrawLine3D(node->pos, node->left->pos, GREEN);
    }

    if (node->right != nullptr)
    {
        if (elevated)
            DrawLine3D(Vector3Add(node->pos, {0.0f, 20.0f * m_maxDepth / (depth + 1) - 20, 0.0f}),
                       Vector3Add(node->right->pos, {0.0f, 20.0f * m_maxDepth / (depth + 2) - 20}), RED);
        else
            DrawLine3D(node->pos, node->right->pos, RED);
    }
    if (inverted && depth <= targetDepth)
        return;

    DebugDrawTree(node->left, depth + 1, targetDepth, inverted, elevated);
    DebugDrawTree(node->right, depth + 1, targetDepth, inverted, elevated);
}

void DeepSearch(KDNode* root, int targetDepth, int currentDepth, std::vector<KDNode*>& result)
{
    if (root == nullptr)
        return;

    if (currentDepth == targetDepth)
    {
        result.push_back(root);
        return;
    }

    DeepSearch(root->left, targetDepth, currentDepth + 1, result);
    DeepSearch(root->right, targetDepth, currentDepth + 1, result);
}

void KDTree::DebugDrawTreeByDepth(KDNode* root, int depth, bool inverted, bool elevated)
{
    std::vector<KDNode *> deepRoots;

    DeepSearch(root, depth, 0, deepRoots);

    for (int i = 0; i < deepRoots.size(); i++)
        DebugDrawTree(deepRoots[i], 0, depth, inverted, elevated);
}

bool KDTree::validateKdTree(KDNode* root, int depth = 0) 
{
    if (root == nullptr) {
        return true;
    }

    if (root->left != nullptr)
        switch (root->SplitAxis) 
        {
            case 0:
                if (root->left->pos.x > root->pos.x) 
                    return false;
                break;
            case 1:
                if (root->left->pos.y > root->pos.y)
                    return false;
                break;
            case 2:
                if (root->left->pos.z > root->pos.z)
                    return false;
                break;
        }

    if (root->right != nullptr)
        switch (root->SplitAxis) 
        {
            case 0:
                if (root->right->pos.x <= root->pos.x)
                    return false;
                break;
            case 1:
                if (root->right->pos.y <= root->pos.y)
                    return false;
                break;
            case 2:
                if (root->right->pos.z <= root->pos.z)
                    return false;
                break;
        }

    return validateKdTree(root->left, depth + 1) && validateKdTree(root->right, depth + 1);
}

bool KDTree::isBalanced(KDNode* root)
{
    if (root == nullptr)
        return true;

    int leftSize = calculateSize(root->left);
    int rightSize = calculateSize(root->right);

    int threshold = 3; // Allow a difference of 3 nodes

    static int s_MaxDisbalance = 0;
    
    int disbalance = std::abs(leftSize - rightSize);

    if (s_MaxDisbalance < disbalance)
        s_MaxDisbalance = disbalance;

    if (disbalance <= threshold && isBalanced(root->left) && isBalanced(root->right))
        return true;

    TraceLog(LOG_INFO, "Max disbalance: %d", s_MaxDisbalance);
    return false;
}