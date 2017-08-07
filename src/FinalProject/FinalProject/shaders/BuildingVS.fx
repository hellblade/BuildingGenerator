#version 130

in vec3 position, colour;
in vec3 normal;
in vec3 textureCoords;
in vec2 topTexture;

uniform mat4 worldMatrix, viewMatrix, projectionMatrix;

flat out vec3 norm;
out vec3 col;
out vec3 textCoords;
out vec3 oPos;
out vec2 topTextureCoords;

void main()
{
    oPos = position;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(position, 1);
    norm = normalize(normal);// *mat3(worldMatrix);
	col = colour;    
    textCoords = textureCoords;
    topTextureCoords = topTexture;
} 
