#pragma once

#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

#define GLSL_VERSION 330

#define MAX_LIGHTS  4 

#include "raylib.h"

// 3D model
struct RenderedModel
{
    RenderedModel(std::string path_model, std::string path_texture, Vector3 position);
    ~RenderedModel();

    Model m_model;
    Texture2D m_texture;
    Vector3 m_position;
};

class Scene_Renderer
{

public:

    Scene_Renderer();
    ~Scene_Renderer();

    // Adds new object to render
    void RenderModel(std::string path_model, std::string path_texture, Vector3 position);
    
    // Renders all objects and lights
    void RenderScene();

    // Unrenders all objects and clears memory
    void ClearScene();

    void InitializeCamera();

private:

    Camera m_camera;

    // Stores all scene models
    Model cube;
    std::vector<RenderedModel*> m_scene_models;
};
