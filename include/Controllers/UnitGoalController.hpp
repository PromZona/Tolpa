#pragma once

#include "IController.hpp"

class UnitGoalController : public IController {
public:
    UnitGoalController();
    ~UnitGoalController();
    void Tick(float deltaTime);
};
