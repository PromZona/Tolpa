#include "game.hpp"
#include "MeshPicking.hpp"
#include <cfloat>

bool MeshPicker::GetTerrainHit()
{
    auto& sceneRenderer = Game::Instance().GetRendererScene();
    auto& sceneManager = Game::Instance().GetSceneManager();

    Model terrainModel = sceneManager.GetModel(ModelType::MAP);

    m_collision = { 0 };
    m_collision.distance = FLT_MAX;
    m_collision.hit = false;
    m_cursorColor = WHITE;

    m_pickingRay = GetMouseRay(GetMousePosition(), sceneRenderer.GetCamera());

    RayCollision boxHitInfo = GetRayCollisionBox(m_pickingRay, GetMeshBoundingBox(terrainModel.meshes[0]));

    if ((boxHitInfo.hit) && (boxHitInfo.distance < m_collision.distance))
    {
        RayCollision meshHitInfo = { 0 };
        for (int m = 0; m < terrainModel.meshCount; m++)
        {
            // NOTE: We consider the model.transform for the collision check but 
            // it can be checked against any transform Matrix, used when checking against same
            // model drawn multiple times with multiple transforms
            meshHitInfo = GetRayCollisionMesh(m_pickingRay, terrainModel.meshes[m], terrainModel.transform);
            if (meshHitInfo.hit)
            {
                // Save the closest hit mesh
                if ((!m_collision.hit) || (m_collision.distance > meshHitInfo.distance)) m_collision = meshHitInfo;
                
                break;  // Stop once one mesh collision is detected, the colliding mesh is m
            }
        }

        if (meshHitInfo.hit)
        {
            m_collision = meshHitInfo;
            m_cursorColor = ORANGE;
            return true;
        }
    }

    return false;
}