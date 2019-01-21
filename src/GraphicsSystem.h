//
//  Copyright � 2018 Alun Evans. All rights reserved.
//
#pragma once
#include "includes.h"
#include "Shader.h"
#include "Components.h"
#include <unordered_map>

struct Geometry {
    GLuint vao;
    GLuint num_tris;
    Geometry() { vao = 0; num_tris = 0;}
    Geometry(int a_vao, int a_tris) : vao(a_vao), num_tris(a_tris) {}
};

struct Material {
    std::string name;
	int shader_id;
	lm::vec3 ambient;
    lm::vec3 diffuse;
    lm::vec3 specular;
    float specular_gloss;
    
    int diffuse_map;
    
    Material() {
        name = "";
		ambient = lm::vec3(0.1f, 0.1f, 0.1f);
        diffuse = lm::vec3(1.0f, 1.0f, 1.0f);
        specular = lm::vec3(1.0f, 1.0f, 1.0f);
        diffuse_map = -1;
        specular_gloss = 80.0f;
    }
};


class GraphicsSystem {
public:
	~GraphicsSystem();
    void init(int window_width, int window_height);
    void update(float dt);
    
	//viewport
	void updateMainViewport(int window_width, int window_height);

    //shaders, materials and textures
	int loadShader(std::string name, std::string vs_path, std::string fs_path);
	int getShaderProgram(std::string name);
    int createMaterial();
    
    //geometry creationg
    int createPlaneGeometry();
    int createGeometryFromFile(std::string filename);
    
    //getters
    Material& getMaterial(int mat_id) { return materials_.at(mat_id); }

    
private:
    //resources
	std::unordered_map<std::string, Shader*> shaders_;
    std::vector<Geometry> geometries_;
    std::vector<Material> materials_;

    
    //rendering
    void renderMeshComponent_(Mesh& comp);
    GLuint current_program_;
    
    //create geometry buffers
    GLuint generateBuffers_(std::vector<float>& vertices,
                            std::vector<float>& uvs,
                            std::vector<float>& normals,
                            std::vector<unsigned int>& indices);
};
