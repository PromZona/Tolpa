#include "../include/NavigationMesh.hpp"

NavMesh::NavMesh(){}

NavMesh::~NavMesh()
{
    navMesh = {0};
    navMeshTriVec.clear();
    UnloadModel(navModel);
}

void NavMesh::CalculateMiddlePoints()
{
    navModel = LoadModel("../resources/3d_objects/NavigationMesh.glb");

    navMesh = navModel.meshes[0];

    vertices = (Vector3*)(navMesh.vertices);

    int indice_index[3];
    Vector3 v[3];
    Vector3 mid;

    for (int i = 0; i < navMesh.triangleCount * 3; i += 3)
    {
        indice_index[0] = navMesh.indices[i];
        indice_index[1] = navMesh.indices[i + 1];
        indice_index[2] = navMesh.indices[i + 2];

        v[0] = vertices[indice_index[0]];
        v[1] = vertices[indice_index[1]];
        v[2] = vertices[indice_index[2]];

        mid = {(v[0].x + v[1].x + v[2].x) / 3.0f,
               (v[0].y + v[1].y + v[2].y) / 3.0f,
               (v[0].z + v[1].z + v[2].z) / 3.0f};

        navMeshTriVec.push_back(TriangleMesh(v, mid));
    }
}

void NavMesh::ConstructMeshGraph()
{
    for (int i = 0; i < navMeshTriVec.size(); i++)
    {
        for (int j = 0; j < navMesh.triangleCount * 3; j += 3)
        {
            Vector3 nV1 = vertices[navMesh.indices[j]];
            Vector3 nV2 = vertices[navMesh.indices[j + 1]];
            Vector3 nV3 = vertices[navMesh.indices[j + 2]];

            int sharedVertices = 0;

            if (navMeshTriVec[i].vertices[0] == nV1 || navMeshTriVec[i].vertices[1] == nV2 || navMeshTriVec[i].vertices[2] == nV3) 
                sharedVertices++;

            if (navMeshTriVec[i].vertices[1] == nV1 || navMeshTriVec[i].vertices[2] == nV2 || navMeshTriVec[i].vertices[0] == nV3) 
                sharedVertices++;

            if (navMeshTriVec[i].vertices[2] == nV1 || navMeshTriVec[i].vertices[0] == nV2 || navMeshTriVec[i].vertices[1] == nV3) 
                sharedVertices++;

            if (sharedVertices >= 2) 
            {
                Vector3 neighbourMid = {(nV1.x + nV2.x + nV3.x) / 3.0f,
                                        (nV1.y + nV2.y + nV3.y) / 3.0f,
                                        (nV1.z + nV2.z + nV3.z) / 3.0f};

                midConnectivityGraph[navMeshTriVec[i].middlePoint].push_back(neighbourMid);
            }
        }
    }
}

void NavMesh::DebugDrawConnectedTriangles()
{
    for (int i = 0; i < navMeshTriVec.size(); i++)
    {
        for (int j = 0; j < midConnectivityGraph[navMeshTriVec[i].middlePoint].size(); j++)
        {
            DrawLine3D(navMeshTriVec[i].middlePoint, midConnectivityGraph[navMeshTriVec[i].middlePoint][j], RED);
        }
    }
}

void NavMesh::DebugDrawGrid()
{
    int indice_index[3];
    for (int i = 0; i < navMesh.triangleCount * 3; i += 3)
    {
        indice_index[0] = navMesh.indices[i];
        indice_index[1] = navMesh.indices[i + 1];
        indice_index[2] = navMesh.indices[i + 2];

        DrawLine3D(vertices[indice_index[0]], vertices[indice_index[1]], WHITE);
        DrawLine3D(vertices[indice_index[1]], vertices[indice_index[2]], WHITE);
        DrawLine3D(vertices[indice_index[0]], vertices[indice_index[2]], WHITE);
    }
}