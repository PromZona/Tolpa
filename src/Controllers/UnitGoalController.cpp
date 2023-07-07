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
			{
				// Goal reached check
				if (Vector3Distance(transformComp.Position, goalComp.PathToGoal[goalComp.PathToGoal.size() - 1]) < 0.5f)
					goalComp.IsActive = false;
			}

			if (!goalComp.IsActive)
			{
				EntityId cityId = cities[GetRandomValue(0, cities.size() - 1)];
				Vector3 cityPosition = ecs.GetComponent<TransformComponent>(cityId)->Position;

				// For finding the closest node we do first comparison agains infinite distance
				float dis = std::numeric_limits<float>::infinity();

				// Find nearest node to the unit position
				Vector3 startClosestNode = navGridTree.FindNearestNode(*treeRoot, transformComp.Position, dis);

				// reset
				dis = std::numeric_limits<float>::infinity();

				// Find nearest node to destination (hardcoded cityPosition, can by anything)
				Vector3 targetClosestNode = navGridTree.FindNearestNode(*treeRoot, cityPosition, dis);

				// Build path between these two closest nodes
				goalComp.PathToGoal = navGrid.FindPath(startClosestNode, targetClosestNode);
				// Add destination itself to path as final point
				goalComp.PathToGoal.push_back(cityPosition);
				// Smooth path (5 is the amount of interpolation steps between each two points)
				goalComp.PathToGoal = navGrid.SmoothPath(goalComp.PathToGoal, 5);
				
				// Set current step in path to 0, meaning startClosestNode
				goalComp.steps = 0;
				// startClosestNode position as goal
				goalComp.GoalPosition = goalComp.PathToGoal[0];
				goalComp.IsActive = true;
			}
			else
			{
				if (Vector3Distance(goalComp.GoalPosition, transformComp.Position) < 0.5f)
				{
					// aligning current position to be exactly on the node
					transformComp.Position = goalComp.GoalPosition;
					// next destination in path step
					goalComp.steps++;

					// ---------------------------------------------------------------------------------------------------
					// ---------------------------- MAY HAVE SOME IMPACT ON PERFORMANCE ----------------------------------

					// It may be dumb but I thought to check if the rest of the path from current reached node is farther than
					// direct path to destination. It may be costly and units coud clip through terrain (solvable?) but
					// (theoretically) it could improve units movement visually
					// again, this would cost some performance so try it after some measurement system is ready?

					// This is snippet of this idea. Works great in some cases, but i still wonder about performance
					// If distance to city is closer than the next point in path 
					if (goalComp.steps < goalComp.PathToGoal.size() - 1 &&
						Vector3Distance(transformComp.Position, goalComp.PathToGoal[goalComp.PathToGoal.size() - 1]) < 
						Vector3Distance(goalComp.GoalPosition, goalComp.PathToGoal[goalComp.steps]))
					{
						// skip the rest of the steps and go directly to the city
						goalComp.steps = goalComp.PathToGoal.size() - 1;
						// This check occurs every time unit reaches next goal point in the path
						// it should not be too costly on performance but potentially a candidate for optimization
					}

					// ---------------------------------------------------------------------------------------------------
					// ---------------------------------------------------------------------------------------------------

					// Set next goal in path
					goalComp.GoalPosition = goalComp.PathToGoal[goalComp.steps];

				}

				movementComp.Direction = Vector3Normalize(Vector3Subtract(goalComp.GoalPosition, 
																		  transformComp.Position));
			}
		}
	}
}