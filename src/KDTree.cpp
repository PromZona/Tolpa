#include "KDTree.hpp"

KDTree::KDTree()
{
    m_maxDepth = 0;
}
KDTree::~KDTree()
{
    DeleteTree(m_root);
}

void KDTree::DeleteTree(KDNode* node)
{
    if (node == nullptr)
        return;

    DeleteTree(node->Left);
    DeleteTree(node->Right);

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

    return 1 + calculateSize(root->Left) + calculateSize(root->Right);
}

// k-d tree axis split condition
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
KDNode* KDTree::ConstructTree(std::vector<Vector3>& nodePoints)
{
    this->m_nodePoints = nodePoints;
    return ConstructTree(this->m_nodePoints, 0);
}

KDNode* KDTree::ConstructTree(std::vector<Vector3>& nodePoints, int depth) 
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

    root->Left = ConstructTree(leftPoints, depth + 1);
    root->Right = ConstructTree(rightPoints, depth + 1);

    if (m_maxDepth < depth)
        m_maxDepth = depth;

    return root;
}

// NNS algorithm for k-d tree
// it kind of works correctly most of the time but in cases
// where target's Y is way above the grid it gets janky (reason? unknown)
Vector3 KDTree::FindNearestNode(KDNode* root, Vector3 target, float& distance)
{
    static Vector3 s_nearest;

    if (root == nullptr)
        return s_nearest;

    if (Vector3Distance(root->Pos, target) <= distance)
    {
        distance = Vector3Distance(root->Pos, target);
        s_nearest = root->Pos;
    }

    switch(root->SplitAxis)
    {
        case 0:
            if (target.x <= root->Pos.x || root->Right == nullptr)
                s_nearest = FindNearestNode(root->Left, target, distance);
            else
                s_nearest = FindNearestNode(root->Right, target, distance);

            break;
        case 1:
            if (target.y <= root->Pos.y || root->Right == nullptr)
                s_nearest = FindNearestNode(root->Left, target, distance);
            else
                s_nearest = FindNearestNode(root->Right, target, distance);
                
            break;
        case 2:
            if (target.z <= root->Pos.z || root->Right == nullptr)
                s_nearest = FindNearestNode(root->Left, target, distance);
            else
                s_nearest = FindNearestNode(root->Right, target, distance);
                
            break;
    }

    DrawLine3D(target, s_nearest, MAGENTA);
    return s_nearest;
}

void KDTree::DebugDrawTree(KDNode* node, int depth, int targetDepth, bool inverted, bool elevated)
{
    if (node == nullptr)
        return;

    if (node->Left != nullptr)
    {
        if (elevated)
            DrawLine3D(Vector3Add(node->Pos, {0.0f, 20.0f * m_maxDepth / (depth + 1) - 20, 0.0f}), 
                       Vector3Add(node->Left->Pos, {0.0f, 20.0f * m_maxDepth / (depth + 2) - 20}), GREEN);
        else
            DrawLine3D(node->Pos, node->Left->Pos, GREEN);
    }

    if (node->Right != nullptr)
    {
        if (elevated)
            DrawLine3D(Vector3Add(node->Pos, {0.0f, 20.0f * m_maxDepth / (depth + 1) - 20, 0.0f}),
                       Vector3Add(node->Right->Pos, {0.0f, 20.0f * m_maxDepth / (depth + 2) - 20}), RED);
        else
            DrawLine3D(node->Pos, node->Right->Pos, RED);
    }
    if (inverted && depth <= targetDepth)
        return;

    DebugDrawTree(node->Left, depth + 1, targetDepth, inverted, elevated);
    DebugDrawTree(node->Right, depth + 1, targetDepth, inverted, elevated);
}

// Returns every node of the tree below set depth
void DeepSearch(KDNode* root, int targetDepth, int currentDepth, std::vector<KDNode*>& result)
{
    if (root == nullptr)
        return;

    if (currentDepth == targetDepth)
    {
        result.push_back(root);
        return;
    }

    DeepSearch(root->Left, targetDepth, currentDepth + 1, result);
    DeepSearch(root->Right, targetDepth, currentDepth + 1, result);
}

void KDTree::DebugDrawTreeByDepth(KDNode* root, int depth, bool inverted, bool elevated)
{
    std::vector<KDNode *> deepRoots;

    DeepSearch(root, depth, 0, deepRoots);

    for (int i = 0; i < deepRoots.size(); i++)
        DebugDrawTree(deepRoots[i], 0, depth, inverted, elevated);
}

// Check if tree is built correctly
// It does not need to be strictly balanced but invalid tree's NNS wont work
bool KDTree::ValidateKdTree(KDNode* root, int depth = 0) 
{
    if (root == nullptr) {
        return true;
    }

    if (root->Left != nullptr)
        switch (root->SplitAxis) 
        {
            case 0:
                if (root->Left->Pos.x > root->Pos.x) 
                    return false;
                break;
            case 1:
                if (root->Left->Pos.y > root->Pos.y)
                    return false;
                break;
            case 2:
                if (root->Left->Pos.z > root->Pos.z)
                    return false;
                break;
        }

    if (root->Right != nullptr)
        switch (root->SplitAxis) 
        {
            case 0:
                if (root->Right->Pos.x <= root->Pos.x)
                    return false;
                break;
            case 1:
                if (root->Right->Pos.y <= root->Pos.y)
                    return false;
                break;
            case 2:
                if (root->Right->Pos.z <= root->Pos.z)
                    return false;
                break;
        }

    return ValidateKdTree(root->Left, depth + 1) && ValidateKdTree(root->Right, depth + 1);
}

// This is kind of useless since our tree is intentionally
// a little unbalanced but its worth keeping to ensure its not TOO unbalanced
// for some reason in the future
bool KDTree::IsBalanced(KDNode* root)
{
    if (root == nullptr)
        return true;

    int leftSize = calculateSize(root->Right);
    int rightSize = calculateSize(root->Right);

    int threshold = 3; // Allow a difference of 3 nodes

    static int s_MaxDisbalance = 0;
    
    int disbalance = std::abs(leftSize - rightSize);

    if (s_MaxDisbalance < disbalance)
        s_MaxDisbalance = disbalance;

    if (disbalance <= threshold && IsBalanced(root->Left) && IsBalanced(root->Right))
        return true;

    TraceLog(LOG_INFO, "Max disbalance: %d", s_MaxDisbalance);
    return false;
}