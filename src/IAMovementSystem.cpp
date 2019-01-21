#include "IAMovementSystem.h"
#include "extern.h"

void IAMovementSystem::init()
{
}

void IAMovementSystem::update(float dt)
{
	auto& ia_components = ECS.getAllComponents<IAPlayer>();
	for (auto &curr_comp : ia_components) {
		updateMovement(dt, curr_comp);
	}
	
}

void IAMovementSystem::updateMovement(float dt, IAPlayer &ia)
{
	Transform& transform = ECS.getComponentFromEntity<Transform>(ia.owner);

	//multiply speeds by delta time 
	float move_speed_dt = ia.move_speed_ * dt;
	float turn_speed_dt = ia.turn_speed_ * dt;

	/*
	if (input[GLFW_MOUSE_BUTTON_LEFT]) {
		//rotate camera just like Free movement
		lm::mat4 R_yaw, R_pitch;
		//yaw - axis is up vector of world
		R_yaw.makeRotationMatrix(mouse.delta_x * turn_speed_dt, lm::vec3(0, 1, 0));
		camera.forward = R_yaw * camera.forward;
		//pitch - axis is strafe vector of camera i.e cross product of cam_forward and up
		lm::vec3 pitch_axis = camera.forward.normalize().cross(lm::vec3(0, 1, 0));
		R_pitch.makeRotationMatrix(mouse.delta_y * turn_speed_dt, pitch_axis);
		camera.forward = R_pitch * camera.forward;

		//rotate camera transform (for FPS colliders
		transform.rotateLocal(mouse.delta_x * turn_speed_dt, lm::vec3(0, 1, 0));
	}
	*/

	//fps control should have five ray colliders assigned
	auto& colliders = ECS.getAllComponents<Collider>();
	Collider& collider_down = colliders[ia.collider_down];
	Collider& collider_forward = colliders[ia.collider_forward];
	Collider& collider_left = colliders[ia.collider_left];
	Collider& collider_right = colliders[ia.collider_right];
	Collider& collider_back = colliders[ia.collider_back];

	collider_forward.direction = ia.lookTo.normalize();
	collider_forward.direction.y = 0;

	collider_back.direction = lm::vec3(-collider_forward.direction.x, 0.0f, -collider_forward.direction.z);

	collider_left.direction = collider_forward.direction.cross(collider_down.direction);
	collider_right.direction = lm::vec3(-collider_left.direction.x, 0.0f, -collider_left.direction.z);

	//collisions and gravity
	//player down ray is always colliding, we need to keep player at 'FPS_height' units above nearest collider
	float dist_above_ground = (transform.position() - collider_down.collision_point).length();
	//collision test # 1
	if (collider_down.colliding && dist_above_ground < ia.height + 0.01f) // if below or on ground
	{
		//say we can jump
		ia.can_jump = true;
		//force player to correct height above ground
		transform.position(transform.position().x, collider_down.collision_point.y + ia.height, transform.position().z);
	}
	else { // we are in the air
		if (ia.jump_force > 0.0) {// slow down jump with time
			ia.jump_force -= ia.jump_force_slowdown * dt;
		}
		else {// clamp force to 0 if it is already below
			ia.jump_force = 0;
		}

		//move player according to jump force and gravity
		transform.translate(0.0f, (ia.jump_force - ia.gravity)*dt, 0.0f);

		//Collision test #2, as we might have moved down since test #1
		dist_above_ground = (transform.position() - collider_down.collision_point).length();
		if (collider_down.colliding && dist_above_ground < ia.height + 0.01f) // if below or on ground
		{
			//force player to correct height
			transform.position(transform.position().x, collider_down.collision_point.y + ia.height, transform.position().z);
		}
	}

	//jump
	if (ia.can_jump && ia.perform_jump == true) {

		//set jump state to false cos we don't want to double/multiple jump
		ia.can_jump = false;

		//add force to jump upwards
		ia.jump_force = ia.jump_initial_force;

		//start jump
		transform.translate(0.0f, ia.jump_force*dt, 0.0f);
	}

	//forward and strafe 
	lm::vec3 forward_dir = ia.lookTo.normalize() * move_speed_dt;
	lm::vec3 strafe_dir = ia.lookTo.cross(lm::vec3(0, 1, 0)) * move_speed_dt;
	//nerf y components because we can't fly in an FPS
	forward_dir.y = 0.0;
	strafe_dir.y = 0.0;
	//now move
	if (ia.go_forward == true && !collider_forward.colliding)
		transform.translate(forward_dir);
	else if (ia.go_forward == true) {
		if (ia.turn_left) {
			if (collider_left.colliding) {
				ia.turn_left = !ia.turn_left;
			} else {
				lm::vec3 left_dir = collider_left.direction.normalize() * move_speed_dt;
				transform.translate(left_dir);
			}
		}

		if (!ia.turn_left) {
			if (collider_right.colliding) {
				ia.turn_left = !ia.turn_left;
			}
			else {
				lm::vec3 rigth_dir = collider_right.direction.normalize() * move_speed_dt;
				transform.translate(rigth_dir);
			}
		}
		

	}
	ia.reset();
}
