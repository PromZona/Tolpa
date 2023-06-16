#include "../include/NavigationMesh.hpp"

NavMesh::NavMesh(){}

NavMesh::~NavMesh()
{
    navMesh = {0};
    UnloadModel(navModel);
}

void NavMesh::ReInitializationCleanup()
{
    if (IsModelReady(navModel))
    {
        UnloadModel(navModel);
        TraceLog(LOG_INFO, "Navigation Mesh:  Model Reloaded");

        if (*kdTreeNavigationNodes.GetRoot() != nullptr)
        {
            kdTreeNavigationNodes.DeleteTree(*kdTreeNavigationNodes.GetRoot());
            TraceLog(LOG_INFO, "Navigation Mesh:  KD-Tree Reloaded");
        }

        if (!connectivityGraph.empty())
        {
            connectivityGraph.clear();
            TraceLog(LOG_INFO, "Navigation Mesh:  Connectivity Graph Reloaded");
        }

        if (!graphNodes.empty())
        {
            graphNodes.clear();
            TraceLog(LOG_INFO, "Navigation Mesh:  Graph Nodes Reloaded");
        }
    }
}

void NavMesh::InitializeNavigationGrid(Model& navModel)
{
    ReInitializationCleanup();

    this->navModel = navModel;

    navMesh = navModel.meshes[0];

    vertices = (Vector3*)(navMesh.vertices);

    int indice_index[3];
    Vector3 v[3];
    Vector3 mid;

    ConstructMeshGraph();
}

void NavMesh::ConstructMeshGraph()
{
    for (int i = 0; i < navMesh.triangleCount * 3; i += 3)
    {
        Vector3 V1 = vertices[navMesh.indices[i]];
        Vector3 V2 = vertices[navMesh.indices[i + 1]];
        Vector3 V3 = vertices[navMesh.indices[i + 2]];

        Vector3 mid = {
            (V1.x + V2.x + V3.x) / 3.0f,
            (V1.y + V2.y + V3.y) / 3.0f,
            (V1.z + V2.z + V3.z) / 3.0f};

        graphNodes.push_back(mid);

        for (int j = 0; j < navMesh.triangleCount * 3; j += 3)
        {
            Vector3 nV1 = vertices[navMesh.indices[j]];
            Vector3 nV2 = vertices[navMesh.indices[j + 1]];
            Vector3 nV3 = vertices[navMesh.indices[j + 2]];

            int sharedVertices = 0;

            if (V1 == nV1 || V2 == nV2 || V3 == nV3) 
                sharedVertices++;

            if (V2 == nV1 || V3 == nV2 || V1 == nV3) 
                sharedVertices++;

            if (V3 == nV1 || V1 == nV2 || V2 == nV3) 
                sharedVertices++;

            if (sharedVertices >= 2) 
            {
                Vector3 neighbourMid = {(nV1.x + nV2.x + nV3.x) / 3.0f,
                                        (nV1.y + nV2.y + nV3.y) / 3.0f,
                                        (nV1.z + nV2.z + nV3.z) / 3.0f};

                connectivityGraph[mid].push_back(neighbourMid);
            }
        }
    }

    KDNode** treeRoot = kdTreeNavigationNodes.GetRoot(); 
    *treeRoot = kdTreeNavigationNodes.constructTree(graphNodes);

    if (kdTreeNavigationNodes.isBalanced(*treeRoot))
        TraceLog(LOG_INFO, "NavMesh kd-tree is balanced");
    else
        TraceLog(LOG_INFO, "NavMesh kd-tree is not balanced");

    if (kdTreeNavigationNodes.validateKdTree(*treeRoot, 0))
        TraceLog(LOG_INFO, "NavMesh kd-tree is valid");
    else
        TraceLog(LOG_INFO, "NavMesh kd-tree is not valid");
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
        TraceLog(LOG_ERROR, "FindPath: START/TARGET NODE NOT FOUND");
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

    TraceLog(LOG_ERROR, "FindPath: NO PATH FOUND");
    return {};
}

void NavMesh::DebugDrawNavMeshGraph()
{
    for (auto& pair : connectivityGraph)
    {
        Vector3 elevation = {0.0f, 1.0f, 0.0f};
        for (int i = 0; i < pair.second.size(); i++)
        DrawLine3D(Vector3Add(pair.first, elevation),
                    Vector3Add(pair.second[i], elevation), RED);
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
    Vector3 elevation = {0.0f, 1.0f, 0.0f};

    for (int i = (step == 0 ? 0 : step - 1); i < path.size() - 1; i++)
    {
        DrawCube(Vector3Add(path[i], elevation), 1.0f, 1.0f, 1.0f, GREEN);
        DrawLine3D(Vector3Add(path[i], elevation), Vector3Add(path[i + 1], elevation), GREEN);
    }
}

#include <string>
void NavMesh::DebugDrawNearestNeighbour(Camera* camera)
{
    float dis = std::numeric_limits<float>::infinity();
    Vector3 nearest = kdTreeNavigationNodes.findNearestNode(*kdTreeNavigationNodes.GetRoot(), m_testPoint, dis);
}