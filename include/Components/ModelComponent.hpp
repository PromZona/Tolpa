#pragma once

#include "raylib.h"

struct ModelComponent
{
    Model m_model;
    BoundingBox m_BBox;

    ModelComponent(const char* modelFilePath)
    {
        m_model = LoadModel(modelFilePath);
        m_BBox = GetMeshBoundingBox(m_model.meshes[0]);
    }

    ~ModelComponent()
    {
        UnloadModel(m_model);
    }
};