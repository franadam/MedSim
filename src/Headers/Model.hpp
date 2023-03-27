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

// Helper to do things while loading...
// https://stackoverflow.com/a/10917945
template<typename R>
inline bool is_ready(std::future<R> const& f)
{
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}

class Model{
private:
	/*  Model Data  */
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	int mNumMeshes;
	int mNumFaces;
	int  m_id;
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

public:
	glm::vec3 m_intersectPosition;
	glm::vec3 m_translation;
	double m_tmin;
	bool m_isSelected;
	std::string m_name;

	/*  Functions   */
	// Constructor, expects a filepath to a 3D model.
	Model(GLchar* path);
	Model(std::pair<const aiScene*, std::string> init);

	// Draws the model, and thus all its meshes
	void Draw(Shader shader);

	int getNumMeshes();
	int getNumFaces();

	void setTransform(glm::mat4 PVM);
	void setPosition(glm::vec3 translation);
	void setPosition(glm::mat4 position);
	glm::mat4 getPositionMatrix();
	glm::vec3 getPositionVector();
	bool computeIntersectAABB(const Ray& ray);
	void computeAABB();
	void transformAABB();
	void DrawBoundingBox(Shader shader, glm::mat4 placement);
	static std::pair<const aiScene*, std::string> loadAIScene(const GLchar* path);
	static std::future<std::pair<const aiScene*, std::string>> preload(const GLchar* path);

};

#endif 