#pragma once
#ifndef _GEOMETRY_
#define _GEOMETRY_


// System Headers
#include "glitter.hpp"

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
	glm::dvec3 direction;
	glm::dvec3 origin;
};
#endif 