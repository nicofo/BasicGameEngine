#pragma once
#include "includes.h"
#include "Shader.h"
#include <vector>

struct TransformNode {
	std::vector<TransformNode> children;
	int trans_id;
	int entity_owner;
	std::string ent_name;
	bool isTop = false;
};

class DebugSystem {
public:
    ~DebugSystem();
    void init();
    void update(float dt);
    
    void setActive(bool a);

	//public imGUI functions
	bool isShowGUI() { return show_imGUI_; };
	void toggleimGUI() { show_imGUI_ = !show_imGUI_; };

	//set picking ray
	void setPickingRay(int mouse_x, int mouse_y, int screen_width, int screen_height);

private:
    //bools to draw or not
    bool draw_grid_;
    bool draw_icons_;
    bool draw_frustra_;
    bool draw_colliders_;
    
    //cube for frustra and boxes
    void createCube_();
    GLuint cube_vao_;
    
    //colliders
    void createRay_();
    GLuint collider_ray_vao_;
    GLuint collider_box_vao_;
    
    //icons
    void createIcon_();
    GLuint icon_vao_;
	GLuint icon_light_texture_;
	GLuint icon_camera_texture_;
    
    //grid
    void createGrid_();
    GLuint grid_vao_;
    GLuint grid_num_indices;
    float grid_colors[12] = {
        0.7f, 0.7f, 0.7f, //grey
        1.0f, 0.5f, 0.5f, //red
        0.5f, 1.0f, 0.5f, //green
        0.5f, 0.5f, 1.0f }; //blue
    
    //shaders
    Shader* grid_shader_;
    Shader* icon_shader_;

	//imGUI
	bool show_imGUI_ = false;
	void updateimGUI_(float dt);

	//picking
	bool can_fire_picking_ray_ = true;
	int ent_picking_ray_;
    
    const char* line_vertex_shader_ =
    "#version 330\n"
    "layout(location = 0) in vec4 a_vertex; \n"
    "uniform mat4 u_mvp;\n"
    "uniform vec3 u_color[4];\n"
    "uniform int u_color_mod;\n"
    "out lowp vec4 v_color;\n"
    "void main() {;\n"
    "    gl_Position = u_mvp * vec4(a_vertex.xyz, 1); \n"
    "   v_color = vec4(u_color[ int(a_vertex.w) + u_color_mod ], 1.0f);\n"
    "}\n";
    
    const char* line_fragment_shader_ =
    "#version 330\n"
    "in vec4 v_color;\n"
    "layout(location = 0) out vec4 fragColor;\n"
    "void main() {\n"
    "    fragColor = v_color;\n"
    "}\n";
    
    const char* icon_vertex_shader_ =
    "#version 330\n"
    "layout(location = 0) in vec3 a_vertex; \n"
    "layout(location = 1) in vec2 a_uv; \n"
    "out vec2 v_uv;\n"
    "uniform mat4 u_mvp;\n"
    "void main() {\n"
    "    gl_Position = u_mvp * vec4(a_vertex, 1); \n"
    "    v_uv = a_uv; \n"
    "}\n";
    
    const char* icon_fragment_shader_ =
    "#version 330\n"
    "in vec2 v_uv;\n"
    "layout(location = 0) out vec4 fragColor;\n"
    "uniform sampler2D u_icon;\n"
    "void main() {\n"
    "    fragColor = texture(u_icon, v_uv);\n"
    "}\n";
    
};

