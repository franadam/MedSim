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
    glm::dvec3 m_intersectPosition = glm::dvec3(0);
    GLuint boundingBoxVAO = 0;

    /*  Functions  */
    // Constructor
	Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, aiAABB aabb, glm::mat4 PVM);
    Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

    // Render the mesh
	void Draw(Shader shader);

    bool isIntersectAABB(const Ray& ray);
    void computeAABB(glm::mat4 PVM);
    void transformAABB(glm::mat4 PVM);
    void draw_bbox();
    bool testIntersect(Ray worldRay);
    void DrawBoundingBox(Shader& shader, glm::mat4 placement);
	GLuint getVAO();

private:
    /*  Render data  */
    GLuint VAO, VBO, EBO;

    /*  Functions    */
    glm::vec3 Mesh::n_inv(glm::vec3 vector);
    // Initializes all the buffer objects/arrays
	void setupMesh();
};
