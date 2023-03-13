#pragma once
#include "../Headers/Model.hpp"

/*  Functions   */
// Constructor, expects a filepath to a 3D model.
Model::Model(GLchar* path)
{
	this->loadModel(path);
	//m_id = uuid()
}

Model::Model(std::pair<const aiScene*, std::string> init)
{
	//m_id = uuid()
	std::cout << "\nprocessing model ... ";

	// Retrieve the directory path of the filepath
	directory = init.second;

	// Process ASSIMP's root node recursively
	const aiScene* scene = init.first;

	// std::cout << "scene " << scene << " root " << scene->mRootNode << "\n" << std::flush;
	this->processNode(scene->mRootNode, scene);

	std::cout << "done";
}

void Model::setTransform(glm::mat4 PVM) {
	m_Transform = PVM;
}

// Draws the model, and thus all its meshes
void Model::Draw(Shader shader)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].Draw(shader);
}

int Model::getNumMeshes()
	{
		return this->mNumMeshes;
	}

int Model::getNumFaces()
	{
		return this->mNumFaces;
	}

bool Model::computeIntersectAABB(const Ray& ray)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		if (this->meshes[i].isIntersectAABB(ray)) 
		{
			m_intersectPosition = this->meshes[i].m_intersectPosition;
			return true;
		}
	return false;
}

void Model::computeAABB()
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].transformAABB(m_Transform);
}

void Model::DrawBoundingBox(Shader shader, glm::mat4 placement)
{
	for (GLuint i = 0; i < this->meshes.size(); i++)
		this->meshes[i].DrawBoundingBox(shader, placement);
}

// Loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
void Model::loadModel(std::string path)
{
	// Read file via ASSIMP 
	static std::mutex assMutex;
	assMutex.lock();
	Assimp::Importer importer;
	assMutex.unlock();

	//aiProcess_GenBoundingBoxes 
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}
	// Retrieve the directory path of the filepath
	this->directory = path.substr(0, path.find_last_of('/'));

	// Process ASSIMP's root node recursively
	this->processNode(scene->mRootNode, scene);
}

// Processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
void Model::processNode(aiNode* node, const aiScene* scene)
{
	// Process each mesh located at the current node
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		this->mNumMeshes = node->mNumMeshes;
		// The node object only contains indices to index the actual objects in the scene. 
		// The scene contains all the data, node is just to keep stuff organized (like relations between nodes).
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		this->meshes.push_back(this->processMesh(mesh, scene));
	}
	// After we've processed all of the meshes (if any) we then recursively process each of the children nodes
	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		this->processNode(node->mChildren[i], scene);
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
	aiAABB aabb;

	// AABB
	aabb = mesh->mAABB;

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
		this->mNumFaces = mesh->mNumFaces;

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

	glm::mat4 PVM = m_Transform;

	// Return a mesh object created from the extracted mesh data
	return Mesh(vertices, indices, textures, aabb, PVM);
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
			texture.id = this->TextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str;
			textures.push_back(texture);
			this->textures_loaded.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}
	return textures;
}

// factory
static Model create(GLchar* path)
{
	return Model(path);
}

// preconstructor !
 std::pair<const aiScene*, std::string> Model::loadAIScene(const GLchar* path)
{
	std::cout << "\nloading model " << path << " ..." << std::flush;

	// Read file via ASSIMP 

	static std::mutex assMutex;
	assMutex.lock();
	Assimp::Importer* importer = new Assimp::Importer();
	assMutex.unlock();

	const aiScene* scene = importer->ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace | aiProcess_GenBoundingBoxes);
	//std::cout << "\n" << "SCENE : " << scene << " ROOT : " << scene->mRootNode << "\n" << std::flush;

	// Check for errors
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
	{
		std::cout << "ERROR::ASSIMP:: " << importer->GetErrorString() << std::endl << std::flush;
		exit(1);
	}

	// Retrieve the directory path of the filepath
	std::string spath = path;
	std::string dir = spath.substr(0, spath.find_last_of('/'));

	std::cout << "\ndone " << path << std::flush;
	return std::make_pair(scene, dir);
}

 std::future<std::pair<const aiScene*, std::string>> Model::preload(const GLchar* path)
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
