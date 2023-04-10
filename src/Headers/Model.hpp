#pragma once
#ifndef _MODEL_
#define _MODEL_

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

// System Headers
#include "glitter.hpp"

// Local Headers
#include "Mesh.hpp"
namespace resource
{

	// Helper to do things while loading...
	// https://stackoverflow.com/a/10917945
	template<typename R>
	inline bool is_ready(std::future<R> const& f)
	{
		return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
	}

	class Model 
	{

	public:
		glm::vec3 m_intersectPosition;
		glm::vec3 m_translation;
		bool m_isSelected;
		std::string m_name;

		/*  Functions   */
		// Constructor, expects a filepath to a 3D model.
		Model(GLchar* path);
		Model(const aiScene* scene, std::string path);

		// Draws the model, and thus all its meshes
		void Draw(Shader shader);
		void DrawBoundingBox(Shader shader, glm::mat4 placement);

		// Loads a model with supported ASSIMP extensions from file
		static const aiScene* loadAIScene(std::string path);
		static std::future<const aiScene*> preload(GLchar* path);

		//AABB
		bool computeIntersectAABB(const Ray& ray);
		void computeAABB();
		void transformAABB();

		int getNumMeshes();
		int getNumFaces();
		glm::mat4 getPositionMatrix();
		glm::vec3 getPositionVector();
		std::string getName();

		void setTransform(glm::mat4 PVM);
		void setPosition(glm::vec3 translation);
		void setPosition(glm::mat4 position);
		
	private:
		/*  Model Data  */
		std::vector<Mesh> meshes;
		std::string m_directory;
		std::string m_path;
		std::vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
		int m_numMeshes;
		int m_numFaces;
		int m_id;
		glm::mat4 m_Transform;
		glm::mat4 m_position;

		/*  Functions   */
		// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
		void loadModel(std::string path);

		// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);


		// Checks all material textures of a given type and loads the textures if they're not loaded yet.
		// The required info is returned as a Texture struct.
		std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
		GLint TextureFromFile(const char* path, std::string directory);
	};
}

#endif 