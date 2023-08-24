#pragma once

#include "raylib.h"

class MeshPicker
{
public:

    bool GetTerrainHit();
    inline RayCollision& GetCollisionPoint() {return m_collision;}
    inline bool IsTerrainHit() {return m_collision.hit;}
    inline Color& GetCursorColor() {return m_cursorColor;}

private:

    Color m_cursorColor;
    RayCollision m_collision;
    Ray m_pickingRay;
};