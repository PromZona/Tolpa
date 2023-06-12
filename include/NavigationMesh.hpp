#pragma once

#include "raylib.h"
#include "raymath.h"
#include "stdio.h"
#include <cmath>
#include <unordered_map>
#include <unordered_set>
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

inline bool operator==(const Vector3& v1, const Vector3& v2) {
    return (v1.x == v2.x) && (v1.y == v2.y) && (v1.z == v2.z);
};

struct TriangleMesh
{
    Vector3 vertices[3];
    Vector3 middlePoint;

    inline TriangleMesh(Vector3 v[3], Vector3 mP)
    {
        vertices[0] = v[0];
        vertices[1] = v[1];
        vertices[2] = v[2];

        middlePoint = mP;
    }
};

class NavMesh
{
    public:

    NavMesh();
    ~NavMesh();

    void CalculateMiddlePoints();
    void DebugDrawGrid();
    void DebugDrawConnectedTriangles();
    void ConstructMeshGraph();

    inline std::vector<TriangleMesh>&  GetTriangles() {return navMeshTriVec;}
    inline Model& GetModel() {return navModel;}

    private:

    Model navModel;
    Mesh navMesh;

    Vector3* vertices;

    // Stores every mesh triangle
    std::vector<TriangleMesh> navMeshTriVec;

    std::unordered_map<Vector3, std::unordered_set<int>, Vector3Hash> connectivityGraph;

    std::unordered_map<Vector3, std::vector<Vector3>, Vector3Hash> midConnectivityGraph;

    std::vector<Vector3> weakVertices;
    std::vector<TriangleMesh> weakTriangles;

    std::vector<Vector3> strongVertices;
};