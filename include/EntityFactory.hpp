#pragma once

#include <string>

class Entity;

class EntityFactory {
public:
<<<<<<< Updated upstream
    static Entity& CreateUnit();
    static Entity& CreateCity();
=======
	static Entity& CreateParty();
	static Entity& CreateCity();
	static Entity& CreateLair();
>>>>>>> Stashed changes
};
