#version 130

in vec2 textCoords;
out vec4 fragColor;
in vec3 pos;

uniform sampler2D texColour;

uniform float xTime;

void main()
{
    vec2 move = vec2(1, 0);
    vec4 perlin = texture2D(texColour, textCoords + xTime * move) / 2;
    perlin += texture2D(texColour, textCoords * 2 + xTime * move) / 4;
    perlin += texture2D(texColour, textCoords * 4 + xTime * move) / 8;
    perlin += texture2D(texColour, textCoords * 8 + xTime * move) / 16;
    perlin += texture2D(texColour, textCoords * 16 + xTime * move) / 32;
    perlin += texture2D(texColour, textCoords * 32 + xTime * move) / 32;

    float cloudValue;
    cloudValue = 1.0 - pow(perlin.r, 0.7) * 2.0;    
          
    vec4 topColor = vec4(0.3f, 0.3f, 0.8f, 1);
    vec4 bottomColor = vec4(1, 1, 1, 1);

    float interp = clamp(pos.y / 0.4, 0.0, 1.0);

    vec4 baseColor = bottomColor * (1 - interp) +  topColor * interp;

    fragColor = baseColor * (1 - cloudValue) + vec4(1, 1, 1, 1) * cloudValue;
}
