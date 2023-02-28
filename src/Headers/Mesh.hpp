#pragma once
#ifndef _MESH_
#define _MESH_
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

// Local Headers
#include "Shader.hpp"


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

class Mesh {
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
	std::vector<Texture> textures;
	std::vector<Face> faces;

    /*  Functions  */
    // Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    // Render the mesh
	void Draw(Shader shader);

	GLuint getVAO();

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
	void setupMesh();
};
