#include "ECS/ECS.hpp"

#include "gtest/gtest.h"

struct ECSTestComponent{int x;};
struct ECSTestComponent1{int y;};
struct ECSTestComponent2{int z;};

TEST(ECSTest, CreateEntity)
{
	ECS ecs{};
	EntityId lastEntity;
	for (int i = 0; i < 10; i++)
	{
		lastEntity = ecs.CreateEntity();
	}
	EXPECT_EQ(lastEntity, 9);
}

TEST(ECSTest, AddGetComponent)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	ecs.AddComponentToEntity(entity, ECSTestComponent{5});
	auto& resultComp = ecs.GetComponent<ECSTestComponent>(entity);
	EXPECT_EQ(resultComp.x, 5);
}

TEST(ECSTest, AddThreeComponents)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	ecs.AddComponentToEntity(entity, ECSTestComponent{5});
	ecs.AddComponentToEntity(entity, ECSTestComponent1{7});
	ecs.AddComponentToEntity(entity, ECSTestComponent2{10});

	auto& resultComp = ecs.GetComponent<ECSTestComponent>(entity);
	auto& resultComp1 = ecs.GetComponent<ECSTestComponent1>(entity);
	auto& resultComp2 = ecs.GetComponent<ECSTestComponent2>(entity);

	// ecs.PrintInfo();
	EXPECT_EQ(resultComp.x, 5);
	EXPECT_EQ(resultComp1.y, 7);
	EXPECT_EQ(resultComp2.z, 10);
}

TEST(ECSTest, GetNonExistentComponent)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	EXPECT_THROW(ecs.GetComponent<ECSTestComponent>(entity), std::out_of_range);
}

TEST(ECSTest, AddComponentsSequentially)
{
	ECS ecs{};
	std::vector<EntityId> entities(10);
	for (int i = 0; i < 10; i++)
		entities[i] = ecs.CreateEntity();
	for (int i = 0; i < 10; i++)
		ecs.AddComponentToEntity<ECSTestComponent>(entities[i], {i});
	for (int i = 0; i < 10; i++)
		ecs.AddComponentToEntity<ECSTestComponent1>(entities[i], {i+1});

	for (int i = 0; i < 10; i++)
	{
		EXPECT_EQ(ecs.GetComponent<ECSTestComponent>(entities[i]).x, i);
		EXPECT_EQ(ecs.GetComponent<ECSTestComponent1>(entities[i]).y, i + 1);
	}
}