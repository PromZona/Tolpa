#include "../include/3DScene_Renderer.hpp"

#include "../include/EntityManager.hpp"
#include "../include/Components/RenderComponent.hpp"
#include "../include/Components/TransformComponent.hpp"

SceneRenderer::SceneRenderer() = default;
SceneRenderer::~SceneRenderer() = default;

void SceneRenderer::InitializeCamera()
{
    m_camera = { 0 };
    m_camera.position = {18.0f, 21.0f, 18.0f};
    m_camera.target = {0.0f, 0.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = 60.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void SceneRenderer::RenderCursorRayCollision(SceneManager* scene_manager)
{
    m_cursor_collision_detector.m_collision = {0};
    m_cursor_collision_detector.m_collision.distance = FLT_MAX;
    m_cursor_collision_detector.m_collision.hit = false;

    Color cursor_color = WHITE;

    m_cursor_collision_detector.m_ray = GetMouseRay(GetMousePosition(), m_camera);

    m_cursor_collision_detector.m_mesh_hit_info = {0};

    for (size_t i = 0; i < scene_manager->CountSceneObjects(); i++)
    {
        // Check for collision with Binding box first
        RayCollision box_hit_info = GetRayCollisionBox(m_cursor_collision_detector.m_ray,
                                                       scene_manager->GetSceneObject(i)->GetObjectModel()->BBox);

        if (box_hit_info.hit && box_hit_info.distance < m_cursor_collision_detector.m_collision.distance)
            for (int m = 0; m < scene_manager->GetSceneObject(i)->GetObjectModel()->m_model.meshCount; m++)
            {
                m_cursor_collision_detector.m_mesh_hit_info = GetRayCollisionMesh(m_cursor_collision_detector.m_ray, 
                                                            scene_manager->GetSceneObject(i)->GetObjectModel()->m_model.meshes[m],
                                                            scene_manager->GetSceneObject(i)->GetObjectModel()->m_model.transform);
                
                
                if (m_cursor_collision_detector.m_mesh_hit_info.hit)
                {
                    if ((!m_cursor_collision_detector.m_collision.hit) || 
                    (m_cursor_collision_detector.m_collision.distance > m_cursor_collision_detector.m_mesh_hit_info.distance))
                    {
                        m_cursor_collision_detector.m_collision = m_cursor_collision_detector.m_mesh_hit_info;
                    }

                    break;
                }
            }

        if (m_cursor_collision_detector.m_mesh_hit_info.hit)
        {
            m_cursor_collision_detector.m_collision = m_cursor_collision_detector.m_mesh_hit_info;
            cursor_color = ORANGE;
        }
    }

    // Visual representation at the point where collision occured
    // Maybe move somewhere else but for now its okay since this render is called inside BeginDrawing()
    if (m_cursor_collision_detector.m_collision.hit)
    {
        DrawSphere(m_cursor_collision_detector.m_collision.point, 0.2f, cursor_color);

        Vector3 normal_end;
        normal_end.x = m_cursor_collision_detector.m_collision.point.x + m_cursor_collision_detector.m_collision.normal.x;    
        normal_end.y = m_cursor_collision_detector.m_collision.point.y + m_cursor_collision_detector.m_collision.normal.y;    
        normal_end.z = m_cursor_collision_detector.m_collision.point.z + m_cursor_collision_detector.m_collision.normal.z;

        DrawLine3D(m_cursor_collision_detector.m_collision.point, normal_end, RED);
    }
}

void SceneRenderer::RenderScene(SceneManager* scene_manager)
{
    UpdateCamera(&m_camera, CAMERA_PERSPECTIVE);
    
    // Camera always focused on center
    m_camera.target = {0.0f, 0.0f, 0.0f};

    auto& entityManager = EntityManager::Instance();
    auto& renders = entityManager.GetComponents<RenderComponent>();
    auto& transforms = entityManager.GetComponents<TransformComponent>();

    BeginDrawing();
    
        ClearBackground(BLACK);
        DrawFPS(5, 5);

        BeginMode3D(m_camera);

        for (auto& rendComp : renders)
        {
            auto& transform = transforms[rendComp.TransformComponentIndex];
            DrawCircle((int)transform.Position.x, (int)transform.Position.y, rendComp.Radius, rendComp.Color);
        }
        
        for (size_t i = 0; i < scene_manager->CountSceneObjects(); i++)
        {
            DrawModel(scene_manager->GetSceneObject(i)->GetObjectModel()->m_model,
                      scene_manager->GetSceneObject(i)->GetObjectPosition(),
                      m_model_render_scale, WHITE);
        }
        
        // Check if cursor ray collides with any mesh on the screen
        RenderCursorRayCollision(scene_manager);

        EndMode3D();

    if (m_cursor_collision_detector.m_collision.hit)
    {
        int ypos = 70;

        DrawText(TextFormat("Distance: %3.2f", m_cursor_collision_detector.m_collision.distance), 10, ypos, 10, WHITE);

        DrawText(TextFormat("Hit Pos: %3.2f %3.2f %3.2f",
                            m_cursor_collision_detector.m_collision.point.x,
                            m_cursor_collision_detector.m_collision.point.y,
                            m_cursor_collision_detector.m_collision.point.z), 10, ypos + 15, 10, WHITE);

        DrawText(TextFormat("Hit Norm: %3.2f %3.2f %3.2f",
                            m_cursor_collision_detector.m_collision.normal.x,
                            m_cursor_collision_detector.m_collision.normal.y,
                            m_cursor_collision_detector.m_collision.normal.z), 10, ypos + 30, 10, WHITE);
        
    }

    EndDrawing();
}