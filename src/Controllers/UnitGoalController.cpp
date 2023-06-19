#include "Controllers/UnitGoalController.hpp"

#include <vector>

#include "Components/GoalComponent.hpp"
#include "Components/TransformComponent.hpp"
#include "Components/MovementComponent.hpp"

#include "game.hpp"

#define RAYMATH_IMPLEMENTATION
#include "raymath.h"

UnitGoalController::UnitGoalController() = default;
UnitGoalController::~UnitGoalController() = default;

void UnitGoalController::Tick(float deltaTime)
{
	auto& ecs = Game::Instance().GetECS();
	auto& cities = Game::Instance().State.HumanCities;
	auto& navGrid = Game::Instance().GetNavGrid();
	auto& navGridTree = navGrid.GetNavKDTree();
	auto& navGridGraph = navGrid.GetNavConnectGraph();
	KDNode** treeRoot = navGridTree.GetRoot();

	if (cities.empty())
		return;

	auto archetypes = ecs.GetRequiredArchetypes(Archetype);

	for (auto archetype : archetypes)
	{
		auto& moves = archetype->GetComponents<MovementComponent>();
		auto& trans = archetype->GetComponents<TransformComponent>();
		auto& goals = archetype->GetComponents<GoalComponent>();

		std::size_t size = moves.size();
		
		for (int i = 0; i < size; i++)
		{
			auto& goalComp = goals[i];
			auto& movementComp = moves[i];
			auto& transformComp = trans[i];

			if (goalComp.IsActive)
				if (goalComp.steps == goalComp.PathToGoal.size())
					goalComp.IsActive = false;

			if (!goalComp.IsActive)
			{

				EntityId cityId = cities[GetRandomValue(0, cities.size() - 1)];
				Vector3 cityPosition = ecs.GetComponent<TransformComponent>(cityId)->Position;

				float dis = std::numeric_limits<float>::infinity();
				Vector3 startClosestNode = navGridTree.FindNearestNode(*treeRoot, transformComp.Position, dis);

				dis = std::numeric_limits<float>::infinity();
				Vector3 targetClosestNode = navGridTree.FindNearestNode(*treeRoot, cityPosition, dis);

				goalComp.PathToGoal = navGrid.FindPath(startClosestNode, targetClosestNode);
				goalComp.PathToGoal.push_back(cityPosition);

				goalComp.steps = 0;
				goalComp.GoalPosition = goalComp.PathToGoal[0];
				goalComp.IsActive = true;
			}
			else
			{
				if (Vector3Distance(goalComp.GoalPosition, transformComp.Position) < 0.5f)
				{
					transformComp.Position = goalComp.GoalPosition;
					goalComp.steps++;
					goalComp.GoalPosition = goalComp.PathToGoal[goalComp.steps];
				}

				movementComp.Direction = Vector3Normalize(Vector3Subtract(goalComp.GoalPosition, 
																		  transformComp.Position));
			}
		}
	}
}