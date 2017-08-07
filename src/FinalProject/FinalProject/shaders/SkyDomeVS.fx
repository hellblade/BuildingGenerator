#version 130

in vec3 position;
in vec2 txtCoords;

uniform mat4 worldMatrix, viewMatrix, projectionMatrix;

out vec2 textCoords;
out vec3 pos;

void main()
{
    pos = position;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * vec4(position, 1);
    textCoords = txtCoords;
}
