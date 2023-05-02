
// System Headers
#include "glitter.hpp"

// Local Headers
//#include "Gui.hpp"

namespace utils {

	// VAO
	GLuint createTriangleVAO();
	GLuint createAxisVAO();
	GLuint createSquareVAO();
	GLuint createCubeVAO();
	GLuint createLightVAO();
	GLuint createSreen2DVAO();
	GLuint createWorldAxisVAO();
	GLuint createQuadVAO();
	GLuint createCubeVAO(glm::vec3 mini, glm::vec3 maxi);
	GLuint createBoundingBoxVAO(aiVector3D mini, aiVector3D maxi);
	GLuint createPosTargetVAO();
	void drawCircle(float cx, float cy, float r, int num_segments);
	void draw_cylinder(GLfloat radius,
		GLfloat height,
		GLubyte R,
		GLubyte G,
		GLubyte B);

	GLuint createVertColVAO_ext(const GLfloat* vertices, size_t sizeVertices,
		const GLfloat* colors, size_t sizeColors);

	GLuint createPosColTexVAOEBO_ext(const GLfloat* vertices, size_t sizeVertices,
		const GLuint* indices, size_t sizeIndices);

	// from https://stackoverflow.com/a/9443982
	template <unsigned int N>
	GLuint createVertColVAO(GLfloat const (&vertices)[N], GLfloat const (&colors)[N])
	{
		return createVertColVAO_ext(vertices, sizeof(vertices), colors, sizeof(colors));
	}

	template <unsigned int M, unsigned int N>
	GLuint createPosColTexVAOEBO(GLfloat const (&vertices)[M], GLuint const (&indices)[N])
	{
		return createPosColTexVAOEBO_ext(vertices, sizeof(vertices), indices, sizeof(indices));
	}
}
