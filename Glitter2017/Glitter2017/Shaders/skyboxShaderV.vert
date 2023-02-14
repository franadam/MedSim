#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 sbProjection;
uniform mat4 sbView;

void main()
{
    TexCoords = aPos;
    vec4 pos = sbProjection * sbView * vec4(0.5*aPos, 1.0);
    gl_Position = pos.xyww;
}  