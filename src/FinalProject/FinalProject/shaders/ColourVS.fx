#version 130

in vec3 position;
in vec3 colour;

uniform mat4 mat;

out vec3 col;

void main()
{
	gl_Position = mat * vec4(position, 1);
	col = colour;
} 
