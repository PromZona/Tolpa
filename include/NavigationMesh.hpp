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
    Vector3 Vertex;
    float G_Score;
    float F_Score;

    AStarNode(const Vector3& _vector3, float _gScore, float _fScore)
        : Vertex(_vector3), G_Score (_gScore), F_Score (_fScore) {}

    bool operator > (const AStarNode& other) const {return F_Score > other.F_Score;}
};

class NavMesh
{
public:

    NavMesh();
    ~NavMesh();

    void InitializeNavigationGrid(Model& navModel);
    void ReInitializationCleanup();
    void DebugDrawWireframe();
    void DebugDrawNavMeshGraph();
    void DebugDrawPath(std::vector<Vector3> path, int step);
    void DebugDrawNearestNeighbour(Camera* camera);
    void ConstructMeshGraph();

    std::vector<Vector3> FindPath(Vector3& start, Vector3& goal); // A* pathfinding

    inline std::vector<Vector3>&  GetGraphNodes() {return m_graphNodes;}
    inline Model& GetModel() {return m_navModel;}
    inline KDTree& GetNavKDTree() {return m_kdTreeNavigationNodes;}
    inline std::unordered_map<Vector3, std::vector<Vector3>, Vector3Hash>& GetNavConnectGraph(){return m_connectivityGraph;}
    inline Vector3& GetTestPoint(){return m_testPoint;}

private:

    Model m_navModel;
    Mesh m_navMesh;

    Vector3* m_vertices;
    Vector3 m_testPoint; // For nearest node neighbour testing

    KDTree m_kdTreeNavigationNodes;

    // Stores every mesh triangle
    std::vector<Vector3> m_graphNodes;
    std::unordered_map<Vector3, std::vector<Vector3>, Vector3Hash> m_connectivityGraph;
};