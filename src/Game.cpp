//
//  Game.cpp
//
//  Copyright � 2018 Alun Evans. All rights reserved.
//

#include "Game.h"
#include "Shader.h"
#include "extern.h"
#include "Parsers.h"
#include "MovePlatformScript.h"
#include "BasicIAScript.h"
#include "PickupScript.h"
#include <functional>


Game::Game() {

}

int createFree(float aspect, ControlSystem& sys) {
	int ent_player = ECS.createEntity("Player");
	Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
	ECS.getComponentFromEntity<Transform>(ent_player).translate(0.0f, 3.0f, 5.0f);
	player_cam.position = lm::vec3(0.0f, 3.0f, 5.0f);
	player_cam.forward = lm::vec3(0.0f, 0.0f, 1.0f);
	player_cam.setPerspective(60.0f*DEG2RAD, aspect, 0.1f, 10000.0f);

	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	sys.control_type = ControlTypeFree;

	return ent_player;
}

int createPlayer(float aspect, ControlSystem& sys) {
    int ent_player = ECS.createEntity("Player");
    Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
    ECS.getComponentFromEntity<Transform>(ent_player).translate(0.0f, 3.0f, 5.0f);
    player_cam.position = lm::vec3(0.0f, 50.0f, 50.0f);
    player_cam.forward = lm::vec3(-1.0f, 0.0f, -1.0f);
    player_cam.setPerspective(60.0f*DEG2RAD, aspect, 0.01f, 10000.0f);
    
    //FPS colliders
    int ent_down_ray = ECS.createEntity("Down Ray");
    Transform& down_ray_trans = ECS.getComponentFromEntity<Transform>(ent_down_ray);
    down_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
    Collider& down_ray_collider = ECS.createComponentForEntity<Collider>(ent_down_ray);
    down_ray_collider.collider_type = ColliderTypeRay;
    down_ray_collider.direction = lm::vec3(0.0,-1.0,0.0);
    down_ray_collider.max_distance = 100.0f;
    
    int ent_left_ray = ECS.createEntity("Left Ray");
    Transform& left_ray_trans = ECS.getComponentFromEntity<Transform>(ent_left_ray);
    left_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
    Collider& left_ray_collider = ECS.createComponentForEntity<Collider>(ent_left_ray);
    left_ray_collider.collider_type = ColliderTypeRay;
    left_ray_collider.direction = lm::vec3(-1.0,0.0,0.0);
    left_ray_collider.max_distance = 2.0f;
    
    int ent_right_ray = ECS.createEntity("Right Ray");
    Transform& right_ray_trans = ECS.getComponentFromEntity<Transform>(ent_right_ray);
    right_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
    Collider& right_ray_collider = ECS.createComponentForEntity<Collider>(ent_right_ray);
    right_ray_collider.collider_type = ColliderTypeRay;
    right_ray_collider.direction = lm::vec3(1.0,0.0,0.0);
    right_ray_collider.max_distance = 2.0f;
    
    int ent_forward_ray = ECS.createEntity("Forward Ray");
    Transform& forward_ray_trans = ECS.getComponentFromEntity<Transform>(ent_forward_ray);
    forward_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
    Collider& forward_ray_collider = ECS.createComponentForEntity<Collider>(ent_forward_ray);
    forward_ray_collider.collider_type = ColliderTypeRay;
    forward_ray_collider.direction = lm::vec3(0.0,0.0,-1.0);
    forward_ray_collider.max_distance = 2.0f;
    
    int ent_back_ray = ECS.createEntity("Back Ray");
    Transform& back_ray_trans = ECS.getComponentFromEntity<Transform>(ent_back_ray);
    back_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
    Collider& back_ray_collider = ECS.createComponentForEntity<Collider>(ent_back_ray);
    back_ray_collider.collider_type = ColliderTypeRay;
    back_ray_collider.direction = lm::vec3(0.0,0.0,1.0);
    back_ray_collider.max_distance = 2.0f;
    
    //set all colliders for FPS control
	sys.FPS_collider_down = ECS.getComponentID<Collider>(ent_down_ray);
	sys.FPS_collider_left = ECS.getComponentID<Collider>(ent_left_ray);
	sys.FPS_collider_right = ECS.getComponentID<Collider>(ent_right_ray);
	sys.FPS_collider_forward = ECS.getComponentID<Collider>(ent_forward_ray);
	sys.FPS_collider_back = ECS.getComponentID<Collider>(ent_back_ray);
    
	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	sys.control_type = ControlTypeFPS;

    return ent_player;
}
	
void printHello() {
	print("hello");
}

//Nothing here yet
void Game::init() {


	//******* INIT SYSTEMS *******

	//init systems except debug, which needs info about scene
	control_system_.init();
	graphics_system_.init(window_width_, window_height_);
	ia_movement_system_.init();
	script_system_.init(&control_system_),
	gui_system_.init(window_width_, window_height_);

    //******** RESOURCES AND LEVEL **********
    
	//level + lights
    Parsers::parseJSONLevel("data/assets/myScene.json", graphics_system_);
	//ECS;
	// player and camera
	createPlayer((float)window_width_ / (float)window_height_, control_system_);
	auto realplayer = createPlayer((float)window_width_ / (float)window_height_, control_system_);


	// ************* CREATE IA ****** -52.8239, -10.3267, -6.56905
	auto iaGhost1 = createIA("ia1", lm::vec3(51.7904f, -10.0037f, 28.4664f), "data/assets/texture/blue.tga");
	BasicIAScript* iaScript1 = new BasicIAScript(iaGhost1, realplayer);
	script_system_.registerScript(iaScript1);

	auto iaGhost2 = createIA("ia2", lm::vec3(-52.8239, -10.3267, -6.56905), "data/assets/texture/red.tga");
	BasicIAScript* iaScript2 = new BasicIAScript(iaGhost2, realplayer);
	script_system_.registerScript(iaScript2);

	//*********** Create Pickups ********
	ECS.total_pickups = 4;

	int pickup1 = createPickup("pickup1", lm::vec3(-52.8239f, -11.0f, -6.56905f));
	PickupScript* pickupScript1 = new PickupScript(pickup1, realplayer);
	script_system_.registerScript(pickupScript1);

	int pickup2 =  createPickup("pickup2", lm::vec3(0.0f, -11.0f, -40.0f));
	PickupScript* pickupScript2 = new PickupScript(pickup2, realplayer);
	script_system_.registerScript(pickupScript2);
	
	int pickup3 = createPickup("pickup3", lm::vec3(47.9173f, -11.0f, 63.8847f));
	PickupScript* pickupScript3 = new PickupScript(pickup3, realplayer);
	script_system_.registerScript(pickupScript3);

	int pickup4 = createPickup("pickup4", lm::vec3(-25.151f, -10.2363f, -18.9087f));
	PickupScript* pickupScript4 = new PickupScript(pickup4, realplayer);
	script_system_.registerScript(pickupScript4);

	// **** counter pickups****
	int multi_glyph = ECS.createEntity("multi glyph");
	GUIText& el_multi = ECS.createComponentForEntity<GUIText>(multi_glyph);
	el_multi.width = 300;
	el_multi.height = 200;
	el_multi.font_face = "data/fonts/arial.ttf";
	el_multi.font_size = 32;
	el_multi.anchor = GUIAnchorTopRight;
	char* text = new char[32];
	sprintf(text, "%d / %d golden boxes", ECS.current_pickups, ECS.total_pickups);
	el_multi.text = text;
	el_multi.color = lm::vec3(1.0, 0.0, 1.0);

	// **** Difficulty change****
	int instruccions_glyph = ECS.createEntity("instruccions");
	GUIText& el_instruccions = ECS.createComponentForEntity<GUIText>(instruccions_glyph);
	el_instruccions.width = 300;
	el_instruccions.height = 200;
	el_instruccions.font_face = "data/fonts/arial.ttf";
	el_instruccions.font_size = 20;
	el_instruccions.anchor = GUIAnchorTopLeft;
	el_instruccions.text = "press:\n (K) IA patrol and follow\n (L) Only follow";
	el_instruccions.color = lm::vec3(1.0, 0.0, 1.0);


	// **** Env****
	ECS.gui_counter_id = multi_glyph;
	ECS.gui_system_ = &gui_system_;
	
    //******* INIT DEBUG SYSTEM *******
	gui_system_.lateInit();
	script_system_.lateInit();
    debug_system_.init();
    debug_system_.setActive(false);
}

//Entry point for game update code
void Game::update(float dt) {
	//update each system in turn

	

	//collision
	collision_system_.update(dt);

	//render
	graphics_system_.update(dt);


    //debug
    debug_system_.update(dt);

	//gui
	gui_system_.update(dt);

	if (!ECS.game_lose && !ECS.game_win) {
		// IA movement
		ia_movement_system_.update(dt);

		//update input
		control_system_.update(dt);

		//scripts
		script_system_.update(dt);
	}
   
}
//update game viewports
void Game::update_viewports(int window_width, int window_height) {
	window_width_ = window_width;
	window_height_ = window_height;

	auto& cameras = ECS.getAllComponents<Camera>();
	for (auto& cam : cameras) {
		cam.setPerspective(60.0f*DEG2RAD, (float)window_width_ / (float) window_height_, 0.01f, 10000.0f);
	}

	graphics_system_.updateMainViewport(window_width_, window_height_);
	gui_system_.updateViewport(window_width_, window_height_);
}

int Game::createPickup(std::string name, lm::vec3 initialPosition) {
	int ent_player = ECS.createEntity(name);
	ECS.getComponentFromEntity<Transform>(ent_player).translate(initialPosition);
	// load a shader into the graphics system
	int body_shader = graphics_system_.loadShader("body_shader", "data/shaders/phong.vert", "data/shaders/phong.frag");

	// load geometry into GraphicsSystem (stored in std::vector)
	int body_geom_id = graphics_system_.createGeometryFromFile("data/assets/cube.obj");

	//manually create material in graphics system (also stored in std::vector), get ref to it
	int body_material_id = graphics_system_.createMaterial();
	Material& body_material = graphics_system_.getMaterial(body_material_id);

	//set shader it to the one we loaded above
	body_material.shader_id = body_shader;

	//set material diffuse map. parseTexture returns an openGL texture id
	body_material.diffuse_map = Parsers::parseTexture("data/assets/texture/yellow.tga");

	Mesh& mesh_comp = ECS.createComponentForEntity<Mesh>(ent_player);
	mesh_comp.geometry = body_geom_id;
	mesh_comp.material = body_material_id;

	return ent_player;
}

int Game::createIA(std::string name, lm::vec3 initialPosition, std::string texturePath) {
	int ent_player = ECS.createEntity("IA " + name);
	auto &iaPlayer = ECS.createComponentForEntity<IAPlayer>(ent_player);
	iaPlayer.move_speed_ = 5.0f;
	ECS.getComponentFromEntity<Transform>(ent_player).translate(initialPosition);
	iaPlayer.lookTo = lm::vec3(0.0, 0.0, 1.0);
	iaPlayer.go_forward = true;

	// load a shader into the graphics system
	int body_shader = graphics_system_.loadShader("body_shader", "data/shaders/phong.vert", "data/shaders/phong.frag");

	// load geometry into GraphicsSystem (stored in std::vector)
	int body_geom_id = graphics_system_.createGeometryFromFile("data/assets/ghost.obj");

	//manually create material in graphics system (also stored in std::vector), get ref to it
	int body_material_id = graphics_system_.createMaterial();
	Material& body_material = graphics_system_.getMaterial(body_material_id);

	//set shader it to the one we loaded above
	body_material.shader_id = body_shader;

	//set material diffuse map. parseTexture returns an openGL texture id
	body_material.diffuse_map = Parsers::parseTexture(texturePath);

	Mesh& mesh_comp = ECS.createComponentForEntity<Mesh>(ent_player);
	mesh_comp.geometry = body_geom_id;
	mesh_comp.material = body_material_id;

	//FPS colliders 
	//each collider ray entity is parented to the playerFPS entity
	int ent_down_ray = ECS.createEntity("Down Ray " + name);
	Transform& down_ray_trans = ECS.createComponentForEntity<Transform>(ent_down_ray);
	down_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& down_ray_collider = ECS.createComponentForEntity<Collider>(ent_down_ray);
	down_ray_collider.collider_type = ColliderTypeRay;
	down_ray_collider.direction = lm::vec3(0.0, -1.0, 0.0);
	down_ray_collider.max_distance = 100.0f;

	int ent_left_ray = ECS.createEntity("Left Ray " + name);
	Transform& left_ray_trans = ECS.createComponentForEntity<Transform>(ent_left_ray);
	left_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& left_ray_collider = ECS.createComponentForEntity<Collider>(ent_left_ray);
	left_ray_collider.collider_type = ColliderTypeRay;
	left_ray_collider.direction = lm::vec3(-1.0, 0.0, 0.0);
	left_ray_collider.max_distance = 1.0f;

	int ent_right_ray = ECS.createEntity("Right Ray " + name);
	Transform& right_ray_trans = ECS.createComponentForEntity<Transform>(ent_right_ray);
	right_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& right_ray_collider = ECS.createComponentForEntity<Collider>(ent_right_ray);
	right_ray_collider.collider_type = ColliderTypeRay;
	right_ray_collider.direction = lm::vec3(1.0, 0.0, 0.0);
	right_ray_collider.max_distance = 1.0f;

	int ent_forward_ray = ECS.createEntity("Forward Ray " + name);
	Transform& forward_ray_trans = ECS.createComponentForEntity<Transform>(ent_forward_ray);
	forward_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& forward_ray_collider = ECS.createComponentForEntity<Collider>(ent_forward_ray);
	forward_ray_collider.collider_type = ColliderTypeRay;
	forward_ray_collider.direction = lm::vec3(0.0, 0.0, -1.0);
	forward_ray_collider.max_distance = 1.0f;

	int ent_back_ray = ECS.createEntity("Back Ray " + name);
	Transform& back_ray_trans = ECS.createComponentForEntity<Transform>(ent_back_ray);
	back_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& back_ray_collider = ECS.createComponentForEntity<Collider>(ent_back_ray);
	back_ray_collider.collider_type = ColliderTypeRay;
	back_ray_collider.direction = lm::vec3(0.0, 0.0, 1.0);
	back_ray_collider.max_distance = 1.0f;

	//the control system stores the FPS colliders 
	iaPlayer.collider_down = ECS.getComponentID<Collider>(ent_down_ray);
	iaPlayer.collider_left = ECS.getComponentID<Collider>(ent_left_ray);
	iaPlayer.collider_right = ECS.getComponentID<Collider>(ent_right_ray);
	iaPlayer.collider_forward = ECS.getComponentID<Collider>(ent_forward_ray);
	iaPlayer.collider_back = ECS.getComponentID<Collider>(ent_back_ray);

	return ent_player;
}


