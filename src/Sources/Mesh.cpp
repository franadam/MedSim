#pragma once

#include "../Headers/Mesh.hpp"
#include "../Headers/util.hpp"

    /*  Functions  */
    // Constructor
Mesh::Mesh(std::vector<Vertex> vertices, std::vector<GLuint> indices, std::vector<Texture> textures, aiAABB aabb, glm::mat4 PVM)
    {

        this->vertices = vertices;
        this->indices = indices;
        this->textures = textures;
        this->m_aabb = aabb;
        //computeAABB(PVM);
        boundingBoxVAO = createBoundingBoxVAO(m_aabb.mMin, m_aabb.mMax);

        // Now that we have all the required data, set the vertex buffers and its attribute pointers.
        this->setupMesh();
    }

void Mesh::transformAABB(glm::mat4 PVM) {

    glm::vec4 mMinw = PVM * glm::vec4(m_aabb.mMin.x, m_aabb.mMin.y, m_aabb.mMin.z, 1);
    glm::vec4 mMaxw = PVM * glm::vec4(m_aabb.mMax.x, m_aabb.mMax.y, m_aabb.mMax.z, 1);

    m_aabb.mMin = aiVector3D(mMinw.x, mMinw.y, mMinw.z);
    m_aabb.mMin = aiVector3D(mMaxw.x, mMaxw.y, mMaxw.z);
}

void Mesh::computeAABB(glm::mat4 PVM) {

    this->m_aabb.mMin.x = +INFINITY;
    this->m_aabb.mMin.y = +INFINITY;
    this->m_aabb.mMin.z = +INFINITY;

    this->m_aabb.mMax.x = -INFINITY;
    this->m_aabb.mMax.y = -INFINITY;
    this->m_aabb.mMax.z = -INFINITY;
    //gl_Position = Projection * View *Model* vec4(vs_out.FragPos, 1.0);
    for (int i = 0; i < vertices.size(); i++)
    {
        if (vertices[i].Position.x < this->m_aabb.mMin.x)
        {
            this->m_aabb.mMin.x = vertices[i].Position.x;
        }
        if (vertices[i].Position.y < this->m_aabb.mMin.y)
        {
            this->m_aabb.mMin.y = vertices[i].Position.y;
        }
        if (vertices[i].Position.z < this->m_aabb.mMin.z)
        {
            this->m_aabb.mMin.z = vertices[i].Position.z;
        }

        if (vertices[i].Position.x > this->m_aabb.mMax.x)
        {
            this->m_aabb.mMax.x = vertices[i].Position.x;
        }
        if (vertices[i].Position.y > this->m_aabb.mMax.y)
        {
            this->m_aabb.mMax.y = vertices[i].Position.y;
        }
        if (vertices[i].Position.z > this->m_aabb.mMax.z)
        {
            this->m_aabb.mMax.z = vertices[i].Position.z;
        }
    }

    glm::vec4 mMinw = PVM * glm::vec4(m_aabb.mMin.x, m_aabb.mMin.y, m_aabb.mMin.z, 1);
    glm::vec4 mMaxw = PVM * glm::vec4(m_aabb.mMax.x, m_aabb.mMax.y, m_aabb.mMax.z, 1);

    m_aabb.mMin = aiVector3D(mMinw.x, mMinw.y, mMinw.z);
    m_aabb.mMin = aiVector3D(mMaxw.x, mMaxw.y, mMaxw.z);

    glm::vec3 size = glm::vec3(m_aabb.mMax.x - m_aabb.mMin.x, m_aabb.mMax.y - m_aabb.mMin.y, m_aabb.mMax.z - m_aabb.mMin.z);
    glm::vec3 center = glm::vec3((m_aabb.mMin.x + m_aabb.mMax.x) / 2, (m_aabb.mMin.y + m_aabb.mMax.y) / 2, (m_aabb.mMin.z + m_aabb.mMax.z) / 2);
    glm::mat4 transform = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);
}

void Mesh::DrawBoundingBox(Shader& shader, glm::mat4 placement)
{
    shader.setMatrix4("Model", placement);
    glBindVertexArray(boundingBoxVAO);
    glDrawArrays(GL_LINES, 0, 24);
    glBindVertexArray(0);
}

    // Render the mesh
    void Mesh::Draw(Shader shader) 
    {
        // Bind appropriate textures
        GLuint diffuseNr = 1;
        GLuint specularNr = 1;
		GLuint normalNr = 1;
		GLuint heightNr = 1;
		GLuint glossNr = 1;
		GLuint ambientNr = 1;
        for(GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
            // Retrieve texture number (the N in diffuse_textureN)
            std::stringstream ss;
            std::string number;
            std::string name = this->textures[i].type;
            if(name == "texture_diffuse")
                ss << diffuseNr++; // Transfer GLuint to stream
            else if(name == "texture_specular")
                ss << specularNr++; // Transfer GLuint to stream
			else if (name == "texture_normal")
				ss << normalNr++; // transfer unsigned int to stream
			else if (name == "texture_height")
				ss << heightNr++; // transfer unsigned int to stream
			else if (name == "texture_gloss")
				ss << glossNr++; // transfer unsigned int to stream
			else if (name == "texture_ambient")
				ss << ambientNr++; // transfer unsigned int to stream
			number = ss.str(); 
            // Now set the sampler to the correct texture unit
            glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
            // And finally bind the texture
            glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
        }
        
        // Also set each mesh's shininess property to a default value (if you want you could extend this to another mesh property and possibly change this value)
        glUniform1f(glGetUniformLocation(shader.ID, "material.shininess"), 16.0f);

        // Draw mesh
        glBindVertexArray(this->VAO);
        glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Always good practice to set everything back to defaults once configured.
        for (GLuint i = 0; i < this->textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i);
            glBindTexture(GL_TEXTURE_2D, 0);
        }
    }

	GLuint Mesh::getVAO()
	{
		return this->VAO;
	}

    /*  Functions    */
    // Initializes all the buffer objects/arrays
    void Mesh::setupMesh()
    {
        // Create buffers/arrays
        glGenVertexArrays(1, &this->VAO);
        glGenBuffers(1, &this->VBO);
        glGenBuffers(1, &this->EBO);

        glBindVertexArray(this->VAO);
        // Load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
        // A great thing about structs is that their memory layout is sequential for all its items.
        // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
        // again translates to 3/2 floats which translates to a byte array.
        glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);  

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

        // Set the vertex attribute pointers
        // Vertex Positions
        glEnableVertexAttribArray(0);	
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
        // Vertex Normals
        glEnableVertexAttribArray(1);	
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
        // Vertex Texture Coords
        glEnableVertexAttribArray(2);	
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));
		// vertex tangent
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// vertex bitangent
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));
		
		glBindVertexArray(0);
    }


    glm::vec3 Mesh::n_inv(glm::vec3 vector) {
        glm::vec3 inv = glm::vec3(1.0 / vector.x, 1.0 / vector.y, 1.0 / vector.z);
        return inv;
    }

    bool Mesh::isIntersectAABB(const Ray& ray)
    {
        double tx1 = (m_aabb.mMin.x - ray.origin.x) / ray.direction.x;
        double tx2 = (m_aabb.mMax.x - ray.origin.x) / ray.direction.x;

        double tmin = glm::min(tx1, tx2);
        double tmax = glm::max(tx1, tx2);

        double ty1 = (m_aabb.mMin.y - ray.origin.y) / ray.direction.y;
        double ty2 = (m_aabb.mMax.y - ray.origin.y) / ray.direction.y;

        tmin = glm::max(tmin, glm::min(ty1, ty2));
        tmax = glm::min(tmax, glm::max(ty1, ty2));

        double tz1 = (m_aabb.mMin.z - ray.origin.z) / ray.direction.z;
        double tz2 = (m_aabb.mMax.z - ray.origin.z) / ray.direction.z;

        tmin = glm::max(tmin, glm::min(tz1, tz2));
        tmax = glm::min(tmax, glm::max(tz1, tz2));

        std::cout << "tmin : " << tmin << std::endl;
        std::cout << "tmax : " << tmax << std::endl;
        std::cout << "max : " << glm::max(0.0, tmin) << std::endl;

        m_intersectPosition = ray.origin + tmin * (ray.direction);

        return tmax >= glm::max(0.0, tmin);

    }
