### La Salle - Universitat Ramon Llull : Master in Advanced Videogame Development
## Engine Programming - Christmas Project Nico Forteza

#### Game objective
Collect all golden box. Avoid to touch the ghosts.

Change dificulty:
- *K* The ghosts will patrol till they find the player. If the player it's out of range, the ghost will return to patrol.
- *L* The ghost knows here is the player and will try to reach him.


### Changes made from the base engine

#### Files created

###### IAMovementSystem
New system that emulates ControlSystem but for every IA entity.

###### BasicIAScript
The ghost behaviour.

###### PickupScript
Basic script for the pickups.

#### Code changes
- Added waypoints to ECS and changed JSON parser to import them.
- Added updateTextures function in GUISystem to update the texts
- Added parameters in ECS to control Pickups and game states.
- Added if statement in game update to perform the game over.
- Creation of the ghosts, pickups and gui in game init.

#### Others
In the scene directory, you can find a Blender which is the game level.
Plus a python addon for Blender that helps you to create the level, object colliders and waypoints. And generates the JSON and all OBJ files fot the scene
