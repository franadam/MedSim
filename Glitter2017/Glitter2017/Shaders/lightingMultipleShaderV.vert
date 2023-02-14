#version 330 core

//values passed by the VAO
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 View;
uniform mat4 Projection;
uniform mat4 Model;

out vec3 Normal;
out vec3 FragPos;  
out vec2 TexCoords;


void main()
{ 
	FragPos = vec3(Model * vec4(1.5 * aPos, 1.0)); // on est dans l'espace world
    gl_Position = Projection * View * vec4(FragPos, 1.0);
	Normal = mat3(transpose(inverse(Model))) * aNormal; 
	TexCoords = aTexCoord; 
} 