#pragma once

#include <string>

class Entity;

class EntityFactory
{
public:
	static Entity& CreateParty();
	static Entity& CreateCity();
	static Entity& CreateLair();
};
