#pragma once
#ifndef _GEOMETRY_
#define _GEOMETRY_

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Assimp Includes
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


#endif 

struct Vertex {
	glm::vec3 Position; // Position
	glm::vec3 Normal; // Normal
	glm::vec2 TexCoords; // TexCoords	
	glm::vec3 Tangent; // tangent 	
	glm::vec3 Bitangent; // bitangent 
};

struct Texture {
	GLuint id;
	std::string type;
	aiString path;
};

struct Face {
	glm::vec3 p1;
	glm::vec3 p2;
	glm::vec3 p3;
}; 

struct Ray
{
	glm::vec3 direction;
	glm::vec3 origine;
};