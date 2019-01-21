#pragma once
#include "includes.h"
#include "Components.h"
#include <map>


//System which manages all our controls
class IAMovementSystem {
public:
	void init();
	void update(float dt);
	void updateMovement(float dt, IAPlayer &ia);

	//public functions to get key and mouse

	//FPS stuff
};