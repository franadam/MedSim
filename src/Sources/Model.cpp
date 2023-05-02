#pragma once
#include "../Headers/Model.hpp"

namespace resource
{
	Model::Model(const aiScene* scene, std::string path)
	{
		//m_id = uuid()
		std::cout << "\nprocessing model ... ";

		m_path = path;

		// Retrieve the name of the filepath
		std::string name = m_path.substr(m_path.find_last_of("/") + 1);
		m_name = name.substr(0, name.find_last_of('.'));
		
		// Retrieve the directory path of the filepath
		m_directory = m_path.substr(0, m_path.find_last_of('/'));

		m_position = glm::mat4(1);

		// Process ASSIMP's root node recursively
		processNode(scene->mRootNode, scene);

		std::cout << "done";
	}

	int Model::getNumMeshes()
	{
		return m_numMeshes;
	}

	int Model::getNumFaces()
	{
		return m_numFaces;
	}

	glm::mat4 Model::getPositionMatrix() {
		return m_position;
	}

	glm::vec3 Model::getPositionVector() {
		return glm::vec3(m_position[3]);
	}

	std::string Model::getName() {
		return m_name;
	}


	void Model::setPosition(glm::mat4 position) {
		m_position = position;
	}

	// Draws the model, and thus all its meshes
	void Model::Draw(Shader shader)
	{
		shader.setMatrix4("Model", m_position);
		for (GLuint i = 0; i < meshes.size(); i++)
			meshes[i].Draw(shader);
	}

	bool Model::computeIntersectAABB(const Ray& ray)
	{
		glm::mat4 inv_position = glm::inverse(m_position);
		Ray ray_model;
		ray_model.origin = inv_position * glm::vec4(ray.origin, 1.0);
		ray_model.direction = inv_position * glm::vec4(ray.direction, 0.0);
		for (GLuint i = 0; i < meshes.size(); i++)
			if (meshes[i].isIntersectAABB(ray_model))
			{
				m_intersectPosition = m_position * glm::vec4(meshes[i].m_intersectPosition, 1.0);
				return true;
			}
		return false;
	}

	void Model::transformAABB()
	{
		for (GLuint i = 0; i < meshes.size(); i++)
			meshes[i].transformAABB(m_position);
			//meshes[i].transformAABB(m_Transform);
	}

	//void Model::DrawBoundingBox(Shader shader, glm::mat4 placement)
	void Model::DrawBoundingBox(Shader shader)
	{
		for (GLuint i = 0; i < meshes.size(); i++)
			meshes[i].DrawBoundingBox(shader, m_position);
	}

	void Model::shiftPosition(glm::vec3 translation) {
		glm::mat4 translate = glm::translate(glm::mat4(1), translation);
		m_position = translate * m_position;
	}

	// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// Process each mesh located at the current node
		for (GLuint i = 0; i < node->mNumMeshes; i++)
		{
			m_numMeshes = node->mNumMeshes;
			// The node object only contains indices to index the actual objects in the scene. 
			// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

			meshes.push_back(processMesh(mesh, scene));
		}
		// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
		for (GLuint i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}

	}

	//intersectionInRange

	Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
	{
		// Data to fill
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		std::vector<Texture> textures;
		std::vector<Face> faces;

		// Walk through each of the mesh's vertices
		for (GLuint i = 0; i < mesh->mNumVertices; i++)
		{

			Vertex vertex;
			glm::vec3 vector; // We declare a placeholder std::vector since assimp uses its own std::vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
			// Positions
			vector.x = mesh->mVertices[i].x;
			vector.y = mesh->mVertices[i].y;
			vector.z = mesh->mVertices[i].z;
			vertex.Position = vector;
			// Normals
			vector.x = mesh->mNormals[i].x;
			vector.y = mesh->mNormals[i].y;
			vector.z = mesh->mNormals[i].z;
			vertex.Normal = vector;
			// Texture Coordinates
			if (mesh->mTextureCoords[0]) // Does the mesh contain texture coordinates?
			{
				glm::vec2 vec;
				// A vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
				// use models where a vertex can have multiple texture coordinates so we always take the first set (0).
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.TexCoords = vec;
			}
			else
				vertex.TexCoords = glm::vec2(0.0f, 0.0f);

			// tangent
			vector.x = mesh->mTangents[i].x;
			vector.y = mesh->mTangents[i].y;
			vector.z = mesh->mTangents[i].z;
			vertex.Tangent = vector;
			// bitangent
			vector.x = mesh->mBitangents[i].x;
			vector.y = mesh->mBitangents[i].y;
			vector.z = mesh->mBitangents[i].z;
			vertex.Bitangent = vector;

			vertices.push_back(vertex);
		}
		// Now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
		for (GLuint i = 0; i < mesh->mNumFaces; i++)
		{
			m_numFaces = mesh->mNumFaces;

			aiFace face = mesh->mFaces[i];
			// Retrieve all indices of the face and store them in the indices std::vector
			for (GLuint j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		// Process materials
		if (mesh->mMaterialIndex >= 0)
		{
			aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
			// We assume a convention for sampler names in the shaders. Each diffuse texture should be named
			// as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
			// Same applies to other texture as the following list summarizes:
			// Diffuse: texture_diffuseN
			// Specular: texture_specularN
			// Normal: texture_normalN

			// 1. Diffuse std::maps
			std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
			textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
			// 2. Specular std::maps
			std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
			textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
			// 3. normal maps
			std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");
			textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
			// 4. height maps
			std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_DISPLACEMENT, "texture_height");
			textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
			// 5. Gloss maps
			std::vector<Texture> glossMaps = loadMaterialTextures(material, aiTextureType_SHININESS, "texture_gloss");
			textures.insert(textures.end(), glossMaps.begin(), glossMaps.end());
			// 6. Ambient maps
			std::vector<Texture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_ambient");
			textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
		}

		// Return a mesh object created from the extracted mesh data
		return Mesh(vertices, indices, textures);
	}

	// Checks all material textures of a given type and loads the textures if they're not loaded yet.
	// The required info is returned as a Texture struct.
	std::vector<Texture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
	{
		std::vector<Texture> textures;
		for (GLuint i = 0; i < mat->GetTextureCount(type); i++)
		{
			aiString str;
			mat->GetTexture(type, i, &str);
			// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
			GLboolean skip = false;
			for (GLuint j = 0; j < textures_loaded.size(); j++)
			{
				if (textures_loaded[j].path == str)
				{
					textures.push_back(textures_loaded[j]);
					skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
					break;
				}
			}
			if (!skip)
			{   // If texture hasn't been loaded already, load it
				Texture texture;
				texture.id = TextureFromFile(str.C_Str(), m_directory);
				texture.type = typeName;
				texture.path = str;
				textures.push_back(texture);
				textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
			}
		}
		return textures;
	}


	// Model loader !
	const aiScene* Model::loadAIScene(std::string path)
	{
		std::cout << "\nloading model " << path << " ..." << std::flush;

		// Read file via ASSIMP 
		static std::mutex assMutex;
		assMutex.lock();
		Assimp::Importer* importer = new Assimp::Importer();
		assMutex.unlock();

		const aiScene* scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		// Check for errors
		if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
		{
			std::cout << "\n\nERROR::ASSIMP:: " << importer->GetErrorString() << "\n\n" << std::flush;
			//exit(1);
			while (true);
		}

		std::cout << "\ndone " << path << std::flush;
		return scene;
	}

	std::future<const aiScene*> Model::preload(GLchar* path)
	{
		return std::async(std::launch::async, loadAIScene, path);
	}

	GLint Model::TextureFromFile(const char* path, std::string directory)
	{
		//Generate texture ID and load texture data 
		std::string filename = std::string(path);
		filename = directory + '/' + filename;
		GLuint textureID;
		glGenTextures(1, &textureID);
		int width, height, components;
		unsigned char* image = stbi_load(filename.c_str(), &width, &height, &components, 0);
		// Assign texture to ID
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);

		// Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
		stbi_image_free(image);
		return textureID;
	}
}
