#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 myColor;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;


void main() {
    gl_Position = Projection * View *Model*vec4(position.xyz, 1.0f);
    myColor = vec4(color.rgb, 1.0f);
}
