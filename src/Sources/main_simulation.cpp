// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//Matrix
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

// Local Headers
#include "../Headers/Gui.hpp"
#include "../Headers/Shader.hpp"
#include "../Headers/Model.hpp"
#include "../Headers/Simulation.hpp"

int main()
{
	Simulation simulation("Medical Training", gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT, gui::GL_VERSION_MAJOR, gui::GL_VERSION_MINOR, false);

	//MAIN LOOP
	while (!gui::getWindowShouldClose())
	{
		//UPDATE INPUT ---
		simulation.render();
		simulation.update();

	}

	return 0;
}

