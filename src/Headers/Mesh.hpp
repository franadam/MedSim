#pragma once
#ifndef _MESH_
#define _MESH_

// System Headers
#include "glitter.hpp"

// Local Headers
#include "Shader.hpp"
#include "Geometry.hpp"


namespace resource
{

    class Mesh 
    {
    public:
        /*  Mesh Data  */
        std::vector<Vertex> vertices;
        std::vector<GLuint> indices;
        std::vector<Texture> textures;
        std::vector<Face> faces;
        GLuint boundingBoxVAO = 0;
        aiAABB m_aabb;
        glm::dvec3 m_intersectPosition = glm::dvec3(0);


        /*  Functions  */
        // Constructor
        Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures);

        // Render the mesh
        void Draw(Shader shader);

        bool isIntersectAABB(const Ray& ray);
        void computeAABB(glm::mat4 PVM);
        void transformAABB(glm::mat4 PVM);
        bool testIntersect(Ray worldRay);
        void DrawBoundingBox(Shader& shader, glm::mat4 placement);
        GLuint getVAO();

    private:
        /*  Render data  */
        GLuint VAO, VBO, EBO;

        /*  Functions    */
        // Initializes all the buffer objects/arrays
        void setupMesh();
    };
}

#endif 