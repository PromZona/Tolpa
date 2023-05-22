#pragma once

#include <string>

class Entity;

class EntityFactory {
public:
    static Entity& CreateUnit();
    static Entity& CreateCity();
};
