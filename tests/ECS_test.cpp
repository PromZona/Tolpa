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
	ecs.AddComponent(entity, ECSTestComponent{ 5 });
	auto resultComp = ecs.GetComponent<ECSTestComponent>(entity);
	EXPECT_EQ(resultComp->x, 5);
}

TEST(ECSTest, AddThreeComponents)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	ecs.AddComponent(entity, ECSTestComponent{ 5 });
	ecs.AddComponent(entity, ECSTestComponent1{ 7 });
	ecs.AddComponent(entity, ECSTestComponent2{ 10 });

	auto resultComp = ecs.GetComponent<ECSTestComponent>(entity);
	auto resultComp1 = ecs.GetComponent<ECSTestComponent1>(entity);
	auto resultComp2 = ecs.GetComponent<ECSTestComponent2>(entity);

	// ecs.PrintInfo();
	EXPECT_EQ(resultComp->x, 5);
	EXPECT_EQ(resultComp1->y, 7);
	EXPECT_EQ(resultComp2->z, 10);
}

TEST(ECSTest, GetNonExistentComponent)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	EXPECT_EQ(ecs.GetComponent<ECSTestComponent>(entity), nullptr);
}

TEST(ECSTest, AddComponentsSequentially)
{
	ECS ecs{};
	std::vector<EntityId> entities(10);
	for (int i = 0; i < 10; i++)
		entities[i] = ecs.CreateEntity();
	for (int i = 0; i < 10; i++)
		ecs.AddComponent<ECSTestComponent>(entities[i], { i });
	for (int i = 0; i < 10; i++)
		ecs.AddComponent<ECSTestComponent1>(entities[i], { i + 1 });

	for (int i = 0; i < 10; i++)
	{
		EXPECT_EQ(ecs.GetComponent<ECSTestComponent>(entities[i])->x, i);
		EXPECT_EQ(ecs.GetComponent<ECSTestComponent1>(entities[i])->y, i + 1);
	}
}

TEST(ECSTest, AddRemoveComponent)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	ecs.AddComponent<ECSTestComponent>(entity, { 1 });
	ecs.RemoveComponent<ECSTestComponent>(entity);
	EXPECT_EQ(ecs.GetComponent<ECSTestComponent>(entity), nullptr);
}

TEST(ECSTest, AddAndRemoveSeveralComponents)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	ecs.AddComponent<ECSTestComponent>(entity, { 1 });
	ecs.AddComponent<ECSTestComponent1>(entity, { 2 });
	ecs.AddComponent<ECSTestComponent2>(entity, { 3 });

	ecs.RemoveComponent<ECSTestComponent>(entity);
	ecs.RemoveComponent<ECSTestComponent1>(entity);

	EXPECT_EQ(ecs.GetComponent<ECSTestComponent>(entity), nullptr);
	EXPECT_EQ(ecs.GetComponent<ECSTestComponent1>(entity), nullptr);

	EXPECT_EQ(ecs.GetComponent<ECSTestComponent2>(entity) == nullptr, false);
	EXPECT_EQ(ecs.GetComponent<ECSTestComponent2>(entity)->z, 3);
}

TEST(ECSTest, RemoveUnexistingComponent)
{
	ECS ecs{};
	auto entity = ecs.CreateEntity();
	EXPECT_NO_THROW(ecs.RemoveComponent<ECSTestComponent>(entity));
}

TEST(ECSTest, AddRemoveMultipleComponentsForMultipleEntites)
{
	ECS ecs{};
	std::vector<EntityId> entities(10);
	for (int i = 0; i < 10; i++)
		entities[i] = ecs.CreateEntity();
	for (int i = 0; i < 10; i++)
		ecs.AddComponent<ECSTestComponent>(entities[i], { i });
	for (int i = 0; i < 10; i++)
		ecs.AddComponent<ECSTestComponent1>(entities[i], { i + 1 });

	for (int i = 0; i < 5; i++)
		ecs.RemoveComponent<ECSTestComponent1>(entities[i]);

	for (int i = 0; i < 10; i++)
	{
		if (i < 5)
			EXPECT_EQ(ecs.GetComponent<ECSTestComponent1>(entities[i]), nullptr);
		else
			EXPECT_EQ(ecs.GetComponent<ECSTestComponent1>(entities[i])->y, i + 1);
	}
}
