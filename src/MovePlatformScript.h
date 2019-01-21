#pragma once
#include "ScriptSystem.h"


class MovePlatformScript : public Script {
public:
	MovePlatformScript(int owner) : Script(owner) {}
	void update(float dt);

	void toggleMove() {
		print("pepe");
		should_move_ = !should_move_;
	}

private:
	bool should_move_ = false;

};

