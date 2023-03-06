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
#include <assimp/postprocess.h>
#include <assimp/scene.h>

// Local Headers
#include "Shader.hpp"
#include "Geometry.hpp"


#endif 

class Mesh {
public:
    /*  Mesh Data  */
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
	std::vector<Texture> textures;
    std::vector<Face> faces;
    aiAABB m_aabb;
     

    /*  Functions  */
    // Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, aiAABB aabb);
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    // Render the mesh
	void Draw(Shader shader);
    bool isIntersectAABB(const Ray& ray);

	GLuint getVAO();

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    // Initializes all the buffer objects/arrays
	void setupMesh();
};
