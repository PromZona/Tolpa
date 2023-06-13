#include "../include/NavigationMesh.hpp"

NavMesh::NavMesh(){}

NavMesh::~NavMesh()
{
    navMesh = {0};
    navMeshTriVec.clear();
    UnloadModel(navModel);
}

void NavMesh::InitializeNavigationGrid(Model& navModel)
{
    printf("Initializing NavGrid\n");
    this->navModel = navModel;

    navMesh = navModel.meshes[0];

    printf("NavMesh vertice count: %d\n", this->navModel.meshes[0].vertexCount);
    printf("NavMesh triangle count: %d\n", this->navModel.meshes[0].triangleCount);

    vertices = (Vector3*)(navMesh.vertices);

    int indice_index[3];
    Vector3 v[3];
    Vector3 mid;

    // should be optimized later (maybe merged into ConstructMeshGraph entirely)
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

    printf("Constructing NavGraph\n");

    ConstructMeshGraph();

    printf("Nav Mesh done\n");

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

                connectivityGraph[navMeshTriVec[i].middlePoint].push_back(neighbourMid);
            }
        }
    }
}

float DistanceToNode(const Vector3& v1, const Vector3& v2)
{
    int dx = v2.x - v1.x;
    int dy = v2.y - v1.y;
    int dz = v2.z - v1.z;

    return sqrt(dx * dx + dy * dy + dz * dz);
}

std::vector<Vector3> NavMesh::FindPath(Vector3& start, Vector3& goal)
{
    // No start or goal nodes in graph
    if (connectivityGraph.find(start) == connectivityGraph.end() || connectivityGraph.find(goal) == connectivityGraph.end())
    {
        printf("start/end nodes not found\n");
        return {};
    }
    auto heuristic = [&](const Vector3& v) {
        // Euclidean distance between vertex v and the goal vertex
        return DistanceToNode(v, goal);
    };

    std::priority_queue<AStarNode, std::vector<AStarNode>, std::greater<AStarNode>> openNodes;

    std::unordered_map<Vector3, Vector3, Vector3Hash> cameFrom;

    std::unordered_map<Vector3, float, Vector3Hash> gScores;

    for (const auto& pairs : connectivityGraph)
    {
        Vector3 origin = pairs.first;
        gScores[origin] = std::numeric_limits<float>::infinity();
    }

    gScores[start] = 0.0f;

    openNodes.push(AStarNode(start, 0.0f, heuristic(start)));

    while (!openNodes.empty())
    {
        AStarNode current = openNodes.top();
        openNodes.pop();

        if (current.vertex == goal)
        {
            std::vector<Vector3> path;
            Vector3 currentVertex = current.vertex;

            while(currentVertex != start)
            {
                path.push_back(currentVertex);
                currentVertex = cameFrom[currentVertex];
            }

            path.push_back(currentVertex);
            std::reverse(path.begin(), path.end());

            return path;
        }

        for (const Vector3& neighbour : connectivityGraph[current.vertex])
        {
            float tentativeScore = current.gScore + DistanceToNode(current.vertex, neighbour);

            if (tentativeScore < gScores[neighbour])
            {
                gScores[neighbour] = tentativeScore;
                float fScore = tentativeScore + heuristic(neighbour);

                openNodes.push(AStarNode(neighbour, tentativeScore, fScore));

                cameFrom[neighbour] = current.vertex;
            }
        }
    }

    printf("No path found\n");
    return {};
}

// Needs to be VERY OPTIMIZED
// this is terrible on performance
void NavMesh::DebugDrawNavMeshGraph()
{
    for (int i = 0; i < navMeshTriVec.size(); i++)
    {
        for (int j = 0; j < connectivityGraph[navMeshTriVec[i].middlePoint].size(); j++)
        {
            Vector3 elevation = {0.0f, 1.0f, 0.0f};
            DrawLine3D(Vector3Add(navMeshTriVec[i].middlePoint, elevation),
                       Vector3Add(connectivityGraph[navMeshTriVec[i].middlePoint][j], elevation), RED);
        }
    }
}

void NavMesh::DebugDrawWireframe()
{
    int indice_index[3];
    for (int i = 0; i < navMesh.triangleCount * 3; i += 3)
    {
        indice_index[0] = navMesh.indices[i];
        indice_index[1] = navMesh.indices[i + 1];
        indice_index[2] = navMesh.indices[i + 2];

        Vector3 elevation = {0.0f, 1.0f, 0.0f};

        DrawLine3D(Vector3Add(vertices[indice_index[0]], elevation),
                   Vector3Add(vertices[indice_index[1]], elevation), WHITE);
        DrawLine3D(Vector3Add(vertices[indice_index[0]], elevation),
                   Vector3Add(vertices[indice_index[2]], elevation), WHITE);
        DrawLine3D(Vector3Add(vertices[indice_index[1]], elevation),
                   Vector3Add(vertices[indice_index[2]], elevation), WHITE);
    }
}

void NavMesh::DebugDrawPath(std::vector<Vector3> path, int step)
{
    for (int i = step; i < path.size(); i++)
        DrawLine3D(path[i], Vector3Add(path[i], {0.0f, 5.0f, 0.0f}), GREEN);
}