#include "../include/3DScene_Renderer.hpp"

#include "../include/EntityManager.hpp"
#include "../include/Components/RenderComponent.hpp"
#include "../include/Components/TransformComponent.hpp"

SceneRenderer::SceneRenderer(){}

SceneRenderer::~SceneRenderer()
{
    UnloadShader(m_shader_light);
}

void SceneRenderer::InitializeCamera()
{
    m_camera = { 0 };
    m_camera.position = {10.0f, 10.0f, 10.0f};
    m_camera.target = {0.0f, 0.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = 60.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void SceneRenderer::ApplyLightingShaderToObjects(SceneManager* scene_manager)
{
    for (int i = 0; i < scene_manager->CountSceneObjects(); i++)
    {
        for (int m = 0; m < scene_manager->GetSceneObject(i)->GetObjectModel()->m_model.materialCount; m++)
            scene_manager->GetSceneObject(i)->GetObjectModel()->m_model.materials[1].shader = m_shader_light;
        
        printf("Material count: %d\n", scene_manager->GetSceneObject(i)->GetObjectModel()->m_model.materialCount);
    }
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

Light CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader)
{
    Light light = { 0 };

    light.enabled = true;
    light.type = type;
    light.position = position;
    light.target = target;
    light.color = color;

    // For setting uniform values in fragment shader
    char enabledName[32] = "sc_light.enabled\0";
    char typeName[32] = "sc_light.type\0";
    char posName[32] = "sc_light.position\0";
    char targetName[32] = "sc_light.target\0";
    char colorName[32] = "sc_light.color\0";
    
    light.enabledLoc = GetShaderLocation(shader, enabledName);
    light.typeLoc = GetShaderLocation(shader, typeName);
    light.posLoc = GetShaderLocation(shader, posName);
    light.targetLoc = GetShaderLocation(shader, targetName);
    light.colorLoc = GetShaderLocation(shader, colorName);

    UpdateLightValues(shader, light);

    return light;
}

void UpdateLightValues(Shader shader, Light light)
{
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.posLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255, 
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
}

void SceneRenderer::RenderScene(SceneManager* scene_manager)
{
    UpdateCamera(&m_camera, CAMERA_PERSPECTIVE);
    
    // Camera always focused on center
    m_camera.target = {0.0f, 0.0f, 0.0f};

    float cameraPos[3] = {m_camera.position.x, m_camera.position.y, m_camera.position.z};
    SetShaderValue(m_shader_light, m_shader_light.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);

    if (IsKeyPressed(KEY_B))
        m_light.enabled = !m_light.enabled;

    UpdateLightValues(m_shader_light, m_light);

    BeginDrawing();
    
        ClearBackground(BLUE);
        DrawFPS(5, 5);

        BeginMode3D(m_camera);

        // Light sphere
        DrawSphereEx(m_light.position, 0.2f, 8, 8, m_light.color);

        for (size_t i = 0; i < scene_manager->CountSceneObjects(); i++)
        {
            DrawModel(scene_manager->GetSceneObject(i)->GetObjectModel()->m_model,
                      scene_manager->GetSceneObject(i)->GetObjectPosition(),
                      1.0f, WHITE);
        }

        // Check if cursor ray collides with any mesh on the screen
        RenderCursorRayCollision(scene_manager);

        EndMode3D();

    if (m_cursor_collision_detector.m_collision.hit)
    {
        int ypos = 70;

        DrawText(TextFormat("Distance: %3.2f", m_cursor_collision_detector.m_collision.distance), 10, ypos, 10, BLACK);

        DrawText(TextFormat("Hit Pos: %3.2f %3.2f %3.2f",
                            m_cursor_collision_detector.m_collision.point.x,
                            m_cursor_collision_detector.m_collision.point.y,
                            m_cursor_collision_detector.m_collision.point.z), 10, ypos + 15, 10, BLACK);

        DrawText(TextFormat("Hit Norm: %3.2f %3.2f %3.2f",
                            m_cursor_collision_detector.m_collision.normal.x,
                            m_cursor_collision_detector.m_collision.normal.y,
                            m_cursor_collision_detector.m_collision.normal.z), 10, ypos + 30, 10, BLACK);
    }

    DrawText(TextFormat("Camera Pos: %3.2f %3.2f %3.2f",
                        m_camera.position.x,
                        m_camera.position.y,
                        m_camera.position.z), 10, 70 + 45, 10, BLACK);
    
    EndDrawing();
}