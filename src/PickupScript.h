#pragma once
#include "includes.h"
#include "ScriptSystem.h"

class PickupScript : public Script {
public:

	PickupScript(int owner, int realPlayerIndex) : Script(owner) {
		realPlayer = realPlayerIndex;
	};
	void init();
	void update(float dt);
	int realPlayer;
	float distanceTrigger = 5.0f;

private:
	Transform* realPlayerTransform;
	Transform*pickupTransform;
};