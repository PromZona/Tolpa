#include "../include/3DScene_Renderer.hpp"

#include "../include/EntityManager.hpp"
#include "../include/Components/RenderComponent.hpp"
#include "../include/Components/TransformComponent.hpp"

Scene_Renderer::Scene_Renderer() = default;
Scene_Renderer::~Scene_Renderer() = default;

RenderedModel::RenderedModel(std::string path_model, std::string path_texture, Vector3 position)
{
    this->m_model = LoadModel(path_model.c_str());
    this->m_texture = LoadTexture(path_texture.c_str());
    this->m_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = this->m_texture;

    this->m_position = position;
}

RenderedModel::~RenderedModel()
{
    UnloadTexture(this->m_texture);
    UnloadModel(this->m_model);
}

void Scene_Renderer::InitializeCamera()
{
    m_camera = { 0 };
    m_camera.position = {18.0f, 21.0f, 18.0f};
    m_camera.target = {0.0f, 0.0f, 0.0f};
    m_camera.up = {0.0f, 1.0f, 0.0f};
    m_camera.fovy = 60.0f;
    m_camera.projection = CAMERA_PERSPECTIVE;
}

void Scene_Renderer::RenderModel(std::string path_model, std::string path_texture, Vector3 position)
{
    m_scene_models.push_back(new RenderedModel(path_model, path_texture, position));
}

void Scene_Renderer::ClearScene()
{
    for (size_t i = 0; i < m_scene_models.size(); i++)
    {
        delete(m_scene_models[i]);
    }

    m_scene_models.clear();
}

void Scene_Renderer::RenderScene()
{
    UpdateCamera(&m_camera, CAMERA_FIRST_PERSON);

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
        
        for (size_t i = 0; i < m_scene_models.size(); i++)
        {
            DrawModel(m_scene_models[i]->m_model, m_scene_models[i]->m_position, 2.0f, WHITE);
        }

        EndMode3D();

    EndDrawing();
}