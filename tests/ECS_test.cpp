#include "ECS/ECS.hpp"

#include "gtest/gtest.h"

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
