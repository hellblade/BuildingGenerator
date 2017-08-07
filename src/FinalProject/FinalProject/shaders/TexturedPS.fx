#version 130

out vec4 fragColor;
in vec2 tex;

uniform sampler2D Texture;

void main()
{
	fragColor = texture2D(Texture, tex);
}