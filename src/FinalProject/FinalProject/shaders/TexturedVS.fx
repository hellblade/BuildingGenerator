#version 130

in vec3 position;
in vec2 texCoord;

out vec2 tex;

uniform mat4 mat;

void main()
{	
	gl_Position = mat * vec4(position, 1);
	tex = texCoord;
}