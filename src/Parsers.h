#pragma once
#include "includes.h"
#include <vector>
#include "GraphicsSystem.h"

struct TGAInfo //stores info about TGA file
{
	GLuint width;
	GLuint height;
	GLuint bpp; //bits per pixel
	GLubyte* data; //bytes with the pixel information
};

class Parsers {
private:
	static TGAInfo* loadTGA(std::string filename);
public:
	static bool parseOBJ(std::string filename, 
						 std::vector<float>& vertices, 
						 std::vector<float>& uvs, 
						 std::vector<float>& normals,
						 std::vector<unsigned int>& indices);
	static GLint parseTexture(std::string filename);
    static bool parseJSONLevel(std::string filename,
                               GraphicsSystem& graphics_system);
};
