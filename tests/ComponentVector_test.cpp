#include "ECS/ComponentVector.hpp"

#include "gtest/gtest.h"

struct CompVecTestComponent {int x, y;};

TEST(ComponentVectorTest, AddAndGetComponent)
{
	EntityId entity = 0;
	ComponentVector<CompVecTestComponent> comp;

	comp.AddComponent(entity, CompVecTestComponent{3, 5});
	auto& result = comp.GetComponent(entity);

	EXPECT_EQ(result.x, 3);
	EXPECT_EQ(result.y, 5);
}

TEST(ComponentVectorTest, GetVectorWithIteration)
{
	ComponentVector<CompVecTestComponent> comp;
	for (int i = 0; i < 3; i++)
		comp.AddComponent(i, CompVecTestComponent{i, i});

	auto& vec = comp.GetVector();
	int i = 0;
	for (auto& testComp : vec)
	{
		EXPECT_EQ(testComp.x, i);
		i++;
	}
}

TEST(ComponentVectorTest, RemoveOneComponent)
{
	EntityId entity = 0;
	ComponentVector<CompVecTestComponent> comp;
	comp.AddComponent(entity, CompVecTestComponent{0, 0});
	comp.RemoveComponent(entity);
	EXPECT_THROW(comp.GetComponent(entity), std::out_of_range);
}

TEST(ComponentVectorTest, RemoveMiddleComponent)
{
	ComponentVector<CompVecTestComponent> comp;
	for (int i = 0; i < 3; i++)
		comp.AddComponent(i, CompVecTestComponent{i, i});

	comp.RemoveComponent(1);
	comp.AddComponent(4, CompVecTestComponent{4, 4});

	EXPECT_EQ(comp.GetComponent(0).x, 0);
	EXPECT_EQ(comp.GetComponent(2).x, 2);
	EXPECT_EQ(comp.GetComponent(4).x, 4);
	EXPECT_THROW(comp.GetComponent(1), std::out_of_range);
}