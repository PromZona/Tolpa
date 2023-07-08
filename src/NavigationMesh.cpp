#include "../include/NavigationMesh.hpp"

NavMesh::NavMesh(){}

NavMesh::~NavMesh()
{
    m_navMesh = {0};
    UnloadModel(m_navModel);
}

void NavMesh::ReInitializationCleanup()
{
    if (IsModelReady(m_navModel))
    {
        UnloadModel(m_navModel);
        TraceLog(LOG_INFO, "Navigation Mesh:  Model Reloaded");

        if (*m_kdTreeNavigationNodes.GetRoot() != nullptr)
        {
            m_kdTreeNavigationNodes.DeleteTree(*m_kdTreeNavigationNodes.GetRoot());
            TraceLog(LOG_INFO, "Navigation Mesh:  KD-Tree Reloaded");
        }

        if (!m_connectivityGraph.empty())
        {
            m_connectivityGraph.clear();
            TraceLog(LOG_INFO, "Navigation Mesh:  Connectivity Graph Reloaded");
        }

        if (!m_graphNodes.empty())
        {
            m_graphNodes.clear();
            TraceLog(LOG_INFO, "Navigation Mesh:  Graph Nodes Reloaded");
        }
    }
}

void NavMesh::InitializeNavigationGrid(Model& navModel)
{
    ReInitializationCleanup();

    this->m_navModel = navModel;

    m_navMesh = navModel.meshes[0];

    m_vertices = (Vector3*)(m_navMesh.vertices);

    int indice_index[3];
    Vector3 v[3];
    Vector3 mid;

    ConstructMeshGraph();
}

void NavMesh::ConstructMeshGraph()
{
    for (int i = 0; i < m_navMesh.triangleCount * 3; i += 3)
    {
        Vector3 V1 = m_vertices[m_navMesh.indices[i]];
        Vector3 V2 = m_vertices[m_navMesh.indices[i + 1]];
        Vector3 V3 = m_vertices[m_navMesh.indices[i + 2]];

        Vector3 mid = {
            (V1.x + V2.x + V3.x) / 3.0f,
            (V1.y + V2.y + V3.y) / 3.0f,
            (V1.z + V2.z + V3.z) / 3.0f};

        m_graphNodes.push_back(mid);

        for (int j = 0; j < m_navMesh.triangleCount * 3; j += 3)
        {
            Vector3 nV1 = m_vertices[m_navMesh.indices[j]];
            Vector3 nV2 = m_vertices[m_navMesh.indices[j + 1]];
            Vector3 nV3 = m_vertices[m_navMesh.indices[j + 2]];

            int sharedVertices = 0;

            if (V1 == nV1 || V2 == nV2 || V3 == nV3) 
                sharedVertices++;

            if (V2 == nV1 || V3 == nV2 || V1 == nV3) 
                sharedVertices++;

            if (V3 == nV1 || V1 == nV2 || V2 == nV3) 
                sharedVertices++;

            if (sharedVertices == 2) 
            {
                Vector3 neighbourMid = {(nV1.x + nV2.x + nV3.x) / 3.0f,
                                        (nV1.y + nV2.y + nV3.y) / 3.0f,
                                        (nV1.z + nV2.z + nV3.z) / 3.0f};

                m_connectivityGraph[mid].push_back(neighbourMid);
            }
        }
    }

    KDNode** treeRoot = m_kdTreeNavigationNodes.GetRoot(); 
    *treeRoot = m_kdTreeNavigationNodes.ConstructTree(m_graphNodes);

    if (m_kdTreeNavigationNodes.IsBalanced(*treeRoot))
        TraceLog(LOG_INFO, "NavMesh kd-tree is balanced");
    else
        TraceLog(LOG_INFO, "NavMesh kd-tree is not balanced");

    if (m_kdTreeNavigationNodes.ValidateKdTree(*treeRoot, 0))
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
    if (m_connectivityGraph.find(start) == m_connectivityGraph.end() || 
        m_connectivityGraph.find(goal) == m_connectivityGraph.end())
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

    for (const auto& pairs : m_connectivityGraph)
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

        if (current.Vertex == goal)
        {
            std::vector<Vector3> path;
            Vector3 currentVertex = current.Vertex;

            while(currentVertex != start)
            {
                path.push_back(currentVertex);
                currentVertex = cameFrom[currentVertex];
            }

            path.push_back(currentVertex);
            std::reverse(path.begin(), path.end());

            return path;
        }

        for (const Vector3& neighbour : m_connectivityGraph[current.Vertex])
        {
            float tentativeScore = current.G_Score + DistanceToNode(current.Vertex, neighbour);

            if (tentativeScore < gScores[neighbour])
            {
                gScores[neighbour] = tentativeScore;
                float fScore = tentativeScore + heuristic(neighbour);

                openNodes.push(AStarNode(neighbour, tentativeScore, fScore));

                cameFrom[neighbour] = current.Vertex;
            }
        }
    }

    TraceLog(LOG_ERROR, "FindPath: NO PATH FOUND");
    return {};
}

Vector3 QuadraticInterpolation(const Vector3& p0, const Vector3& p1, const Vector3& p2, float t) 
{
    float tSquared = t * t;
    float oneMinusT = 1.0f - t;
    float oneMinusTSquared = oneMinusT * oneMinusT;

    float x = (oneMinusTSquared * p0.x) + (2.0f * oneMinusT * t * p1.x) + (tSquared * p2.x);
    float y = (oneMinusTSquared * p0.y) + (2.0f * oneMinusT * t * p1.y) + (tSquared * p2.y);
    float z = (oneMinusTSquared * p0.z) + (2.0f * oneMinusT * t * p1.z) + (tSquared * p2.z);

    return { x, y, z };
}

std::vector<Vector3> NavMesh::SmoothPath(const std::vector<Vector3>& path, int resolution) 
{
    std::vector<Vector3> smoothedPath;

    if (path.size() < 3)
        return path;

    for (size_t i = 0; i < path.size() - 2; i += 2) 
    {
        Vector3 p0 = path[i];
        Vector3 p1 = path[i + 1];
        Vector3 p2 = path[i + 2];

        for (int j = 0; j <= resolution; j++) 
        {
            float t = static_cast<float>(j) / resolution;
            Vector3 point = QuadraticInterpolation(p0, p1, p2, t);
            smoothedPath.push_back(point);
        }
    }

    size_t lastPointIndex = path.size() - 1;
    if (path.size() % 2 == 0) 
    {
        smoothedPath.push_back(path[lastPointIndex - 1]);
        smoothedPath.push_back(path[lastPointIndex]);
    }
    else
        smoothedPath.push_back(path[lastPointIndex]);

    return smoothedPath;
}

void NavMesh::DebugDrawNavMeshGraph()
{
    for (auto& pair : m_connectivityGraph)
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
    for (int i = 0; i < m_navMesh.triangleCount * 3; i += 3)
    {
        indice_index[0] = m_navMesh.indices[i];
        indice_index[1] = m_navMesh.indices[i + 1];
        indice_index[2] = m_navMesh.indices[i + 2];

        Vector3 elevation = {0.0f, 1.0f, 0.0f};

        DrawLine3D(Vector3Add(m_vertices[indice_index[0]], elevation),
                   Vector3Add(m_vertices[indice_index[1]], elevation), WHITE);
        DrawLine3D(Vector3Add(m_vertices[indice_index[0]], elevation),
                   Vector3Add(m_vertices[indice_index[2]], elevation), WHITE);
        DrawLine3D(Vector3Add(m_vertices[indice_index[1]], elevation),
                   Vector3Add(m_vertices[indice_index[2]], elevation), WHITE);
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

#include "game.hpp"
void NavMesh::DebugDrawNearestNeighbour(Camera* camera)
{
    auto& terrainPicker = Game::Instance().GetRendererScene().GetMeshPicker();
    float dis = std::numeric_limits<float>::infinity();

    auto& hitPoint = terrainPicker.GetCollisionPoint();
    if (terrainPicker.IsTerrainHit())
    {
        Vector3 nearest = m_kdTreeNavigationNodes.FindNearestNode(*m_kdTreeNavigationNodes.GetRoot(), hitPoint.point, dis);
        DrawLine3D(hitPoint.point, nearest, MAGENTA);
        DrawCubeWires(nearest, 2.0f, 2.0f, 2.0f, GREEN);
    }
}