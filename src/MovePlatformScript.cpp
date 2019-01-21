#include "MovePlatformScript.h"
#include "extern.h"

/* MovePlatformScript*/
void MovePlatformScript::update(float dt) {

	if (input_->GetKey(GLFW_KEY_U)) {
		should_move_ = true;
	}
	else if (input_->GetKey(GLFW_KEY_J)) {
		should_move_ = false;
	}

	if (should_move_) {
		auto& transform = ECS.getComponentFromEntity<Transform>(owner_);
		transform.translate(0.0, 1 * dt, 0.0);
	}
}

