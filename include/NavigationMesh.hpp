#pragma once

#include "raylib.h"
#include "raymath.h"
#include "KDTree.hpp"
#include "stdio.h"
#include <cmath>
#include <unordered_map>
#include <queue>
#include <functional>
#include <vector>
#include <algorithm>

struct Vector3Hash {
    std::size_t operator()(const Vector3& v) const {
        std::size_t hashX = std::hash<float>{}(v.x);
        std::size_t hashY = std::hash<float>{}(v.y);
        std::size_t hashZ = std::hash<float>{}(v.z);
        return hashX ^ (hashY << 1) ^ (hashZ << 2);
    }
};

float DistanceToNode(const Vector3& v1, const Vector3& v2);

struct AStarNode
{
    Vector3 vertex;
    float gScore;
    float fScore;

    AStarNode(const Vector3& _vector3, float _gScore, float _fScore)
        : vertex(_vector3), gScore (_gScore), fScore (_fScore) {}

    bool operator > (const AStarNode& other) const {return fScore > other.fScore;}
};

class NavMesh
{
    public:

    NavMesh();
    ~NavMesh();

    void InitializeNavigationGrid(Model& navModel);
    void DebugDrawWireframe();
    void DebugDrawNavMeshGraph();
    void DebugDrawPath(std::vector<Vector3> path, int step);
    void DebugDrawNearestNeighbour(Camera* camera);
    void ConstructMeshGraph();

    std::vector<Vector3> FindPath(Vector3& start, Vector3& goal); // A* pathfinding

    inline std::vector<Vector3>&  GetGraphNodes() {return graphNodes;}
    inline Model& GetModel() {return navModel;}
    inline KDTree& GetNavKDTree() {return kdTreeNavigationNodes;}
    inline std::unordered_map<Vector3, std::vector<Vector3>, Vector3Hash>& GetNavConnectGraph(){return connectivityGraph;}
    inline Vector3& GetTestPoint(){return m_testPoint;}

    private:

    Model navModel;
    Mesh navMesh;

    Vector3* vertices;
    Vector3 m_testPoint; // For nearest node neighbour testing

    KDTree kdTreeNavigationNodes;

    // Stores every mesh triangle
    std::vector<Vector3> graphNodes;
    std::unordered_map<Vector3, std::vector<Vector3>, Vector3Hash> connectivityGraph;
};