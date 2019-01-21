#pragma once
#include "includes.h"
#include "ScriptSystem.h"

class BasicIAScript : public Script {
public:

	BasicIAScript(int owner, int realPlayerIndex) : Script(owner) {
		realPlayer = realPlayerIndex; 
		actualPatrollWaypoint = -1;
	};
	void init();
	void update(float dt);
	int realPlayer;
	bool followPlayer;
	float distanceTrigger = 40.0f;

private:
	int actualPatrollWaypoint;
	int latestPatrolWaypoint;
	IAPlayer* iaPlayer;
	Transform* iaPlayerTransform;
	Transform* realPlayerTransform;

};