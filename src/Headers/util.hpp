#include <iostream>
#include <cstdio>
#include <cstdlib>

#include <assimp/postprocess.h>
#include "../Headers/Gui.hpp"

// VAO
GLuint createTriangleVAO();
GLuint createAxisVAO();
GLuint createSquareVAO();
GLuint createCubeVAO();
GLuint createLightVAO();
GLuint createSreen2DVAO();
GLuint createWorldAxisVAO();
GLuint createQuadVAO();
GLuint createBoundingBoxVAO(aiVector3D mini, aiVector3D maxi);
GLuint createPosTargetVAO();
void drawCircle(float cx, float cy, float r, int num_segments);
void draw_cylinder(GLfloat radius,
	GLfloat height,
	GLubyte R,
	GLubyte G,
	GLubyte B);

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
