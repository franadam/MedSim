#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;

out vec3 ourColor;
 
uniform mat4 Projection;
uniform mat4 View;
uniform mat4 Model;


 void main()
 {
	gl_Position = Projection * View * vec4(100 * aPos, 1.0);
    ourColor = aColor;
 }