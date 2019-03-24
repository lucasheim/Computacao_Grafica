#version 330 core

layout ( location = 0 ) in vec3 vPosition;
layout ( location = 1 ) in vec3 vColors;

out vec3 color;

void main()
{
	color = vColors;
	gl_Position = vec4(vPosition, 1.0);
}