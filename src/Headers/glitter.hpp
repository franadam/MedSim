#pragma once

// Preprocessor Directives
#ifndef _GLITTER_
#define _GLITTER_

// Std. Includes
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <string>
#include <set>
#include <future>
#include <utility>
#include <thread>
#include <mutex>
#include <regex>

// GL Includes
#include <glad/glad.h>
#include <GLFW/glfw3.h>

//Matrix
#include <glm/glm.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
#include <glm/ext/matrix_projection.hpp> // unProject

// Assimp Includes
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#endif