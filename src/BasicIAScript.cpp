 #include "BasicIAScript.h"
#include "extern.h"
void BasicIAScript::init()
{
	realPlayerTransform = &ECS.getComponentFromEntity<Transform>(realPlayer);
	iaPlayer = &ECS.getComponentFromEntity<IAPlayer>(owner_);
	iaPlayerTransform = &ECS.getComponentFromEntity<Transform>(owner_);
	followPlayer = false;
}

// Find the nearest waypoint to the given position
int findNearestWaypoint(lm::vec3 position) {
	float distance = position.distance(ECS.waypoints[0].position);
	int index = 0;
	for (int i = 1; i < ECS.waypoints.size(); i++) {
		float temp_distance = position.distance(ECS.waypoints[i].position);
		if (temp_distance < distance) {
			distance = temp_distance;
			index = i;
		}
	}
	return index;
}


// BFS algorithm to find the nex waypoint to go
int nextWaypointToFindPlayer(int initial_waypoint, int target_waypoint) {
	vector<int> visited;
	for (int i = 0; i < ECS.waypoints.size(); i++)
		visited.push_back(-1);

	// Create a queue for BFS 
	list<int> queue;
	int actual_waypoint = initial_waypoint;
	// Mark the current node as visited and enqueue it 
	visited[actual_waypoint] = actual_waypoint;
	queue.push_back(actual_waypoint);

	// 'i' will be used to get all adjacent 
	// vertices of a vertex 
	list<int>::iterator i;

	while (!queue.empty())
	{
		// Dequeue a vertex from queue and print it 
		actual_waypoint = queue.front();
		queue.pop_front();
		if (actual_waypoint == target_waypoint) {
			break;
		}
		// Get all adjacent vertices of the dequeued 
		// vertex s. If a adjacent has not been visited,  
		// then mark it visited and enqueue it 
		for (int i : ECS.waypoints[actual_waypoint].next_index)
		{
			if (visited[i] < 0)
			{
				visited[i] = actual_waypoint;
				queue.push_back(i);
			}
		}
	}
	int child = target_waypoint;
	int parent = visited[child];
	if (parent < 0) {
		return -1;
	}

	while (parent != initial_waypoint) {
		child = parent;
		parent = visited[child];
	}
	return child;
}


// Lose Title
void endGame() {
	int multi_glyph = ECS.createEntity("lose glyph");
	GUIText& el_multi = ECS.createComponentForEntity<GUIText>(multi_glyph);

	// set text properties
	el_multi.width = 300;
	el_multi.height = 200;
	el_multi.font_face = "data/fonts/arial.ttf";
	el_multi.font_size = 40;
	el_multi.text = "YOU LOSE";
	el_multi.color = lm::vec3(1.0, 0.0, 1.0);
	ECS.gui_system_->lateInit();
	ECS.game_lose = true;
}


void BasicIAScript::update(float dt)
{	
	// initial waypoint
	if (actualPatrollWaypoint < 0) {
		actualPatrollWaypoint = findNearestWaypoint(iaPlayerTransform->position());
	}

	// **** Check if player is near to attack or patrol
	float distance_to_player = realPlayerTransform->position().distance(iaPlayerTransform->position());

	if (distance_to_player < 1.0f) {
		// player catched
		endGame();
	}
	else if (distance_to_player < 40.0f) {
		followPlayer = true;
	}
	else if (distance_to_player > 60.0f) {
		followPlayer = false;
	}



	// **** PATROL*****
	if (!followPlayer) {
		// Init Select the first waypaint to start patrol
		if (actualPatrollWaypoint < 0) {
			actualPatrollWaypoint = findNearestWaypoint(iaPlayerTransform->position());
		}
		auto wp = ECS.waypoints[actualPatrollWaypoint];

		auto distanceToPatrollWaypoint = wp.position.distance(iaPlayerTransform->position());

		// if the entity is in the waypoint,  change  to another waypoint  
		if (distanceToPatrollWaypoint < 3.0f) {

			if (wp.next_index.size() == 0) {
				actualPatrollWaypoint = latestPatrolWaypoint;
				// if only there is a next waypoint
			}
			else if (wp.next_index.size() < 2) {
				actualPatrollWaypoint = wp.next_index[0];

				// Select random next waypoint
			}
			else {
				int newWaypoint = latestPatrolWaypoint;
				while (newWaypoint == latestPatrolWaypoint) {
					int random_index = (rand() % static_cast<int>(wp.next_index.size()));
					newWaypoint = wp.next_index[random_index];
				}
				latestPatrolWaypoint = actualPatrollWaypoint;
				actualPatrollWaypoint = newWaypoint;
			}
			wp = ECS.waypoints[actualPatrollWaypoint];
		}
		auto direction = wp.position - iaPlayerTransform->position();
		iaPlayer->lookTo = direction.normalize();
		iaPlayer->go_forward = true;
	}

	// **** ATTACK/ Follow player ****
	else {
		int nearest_player_waypoint = findNearestWaypoint(realPlayerTransform->position());
		// near player. Free moviment
		if (actualPatrollWaypoint == nearest_player_waypoint) {
			auto direction = realPlayerTransform->position() - iaPlayerTransform->position();
			iaPlayer->lookTo = direction.normalize();
			iaPlayer->go_forward = true;
		}
		else {
			auto wp = ECS.waypoints[actualPatrollWaypoint];
			auto distanceToPatrollWaypoint = wp.position.distance(iaPlayerTransform->position());
			// if the entity is in the waypoint,  change  to another waypoint  
			if (distanceToPatrollWaypoint < 3.0f) {
				actualPatrollWaypoint = nextWaypointToFindPlayer(actualPatrollWaypoint, nearest_player_waypoint);
				wp = ECS.waypoints[actualPatrollWaypoint];
			}
			auto direction = wp.position - iaPlayerTransform->position();
			iaPlayer->lookTo = direction.normalize();
			iaPlayer->go_forward = true;
		}

	}

	/*
	if (followPlayer) {
		auto direction = realPlayerTransform->position() - iaPlayerTransform->position();
		iaPlayer->lookTo = direction.normalize();
		if (distance >=  1.0f) {
			iaPlayer->go_forward = true;
		}
		else {
			int multi_glyph = ECS.createEntity("lose glyph");
			GUIText& el_multi = ECS.createComponentForEntity<GUIText>(multi_glyph);

			// set text properties
			el_multi.width = 300;
			el_multi.height = 200;
			el_multi.font_face = "data/fonts/arial.ttf";
			el_multi.font_size = 40;
			el_multi.text = "YOU LOSE";
			el_multi.color = lm::vec3(1.0, 0.0, 1.0);
			ECS.gui_system_->lateInit();
			ECS.game_lose = true;
		}
	} else {
		auto distanceToPatrollPosition = patrollPositions[actualPatrollPosition].distance(iaPlayerTransform->position());
		if (distanceToPatrollPosition < 13.0f) {
			actualPatrollPosition = (actualPatrollPosition + 1) % patrollPositions.size();
		}
		auto direction = patrollPositions[actualPatrollPosition] - iaPlayerTransform->position();
		iaPlayer->lookTo = direction.normalize();
		iaPlayer->go_forward = true;
	}
	*/

}
