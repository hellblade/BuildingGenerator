#version 130

flat in vec3 norm;
in vec3 oPos;
in vec3 col;
in vec3 textCoords;
in vec2 topTextureCoords;

out vec4 fragColor;
uniform sampler2D texColour, texNormal, texRoof, texRoofNormal;

uniform vec3 lightDir;
uniform vec3 ambient;

void main()
{      
    vec2 txtCoords;
    vec3 tangent, binormal;

    binormal = vec3(0, 1, 0);
    bool isTop;
    isTop = false;

    if (abs(norm.x) == 1)
    {
        txtCoords = vec2(textCoords.z, textCoords.y);        
    }
    else if (abs(norm.z) == 1)
    {
        txtCoords = vec2(textCoords.x, textCoords.y);        
    }
    else if (abs(norm.y) == 1)
    {
        txtCoords.xy = topTextureCoords.xy;
        //txtCoords = vec2(textCoords.x, textCoords.z);
        binormal = vec3(0, 0, 1);
        isTop = true;
    }
    else
    {
        txtCoords = vec2(max(textCoords.x, textCoords.z), textCoords.y);
    }

    vec4 textureColour;
    vec4 bumpMap;

    if (isTop)
    {
        textureColour = texture2D(texRoof, txtCoords) * 0.5f;
        bumpMap = texture2D(texRoofNormal, txtCoords);
    }
    else
    {
        textureColour = texture2D(texColour, txtCoords);
        bumpMap = texture2D(texNormal, txtCoords);
    }

    bumpMap = bumpMap * 2.0 - 1.0;
    tangent = normalize(cross(norm, binormal));

    vec3 bumpNormal = (bumpMap.x * tangent) + (bumpMap.y * binormal) + (bumpMap.z * norm);
    bumpNormal = normalize(bumpNormal);
    
    vec3 V = vec3(normalize(-oPos)); // we are in Eye Coordinates, so EyePos is (0,0,0)  
    vec3 R;
 
    
    vec4 light = vec4(ambient, 1); 
    float intensity = max(dot(bumpNormal, lightDir), 0);

    if (intensity > 0.0)
    {
        light = light + intensity;
        light = clamp(light, 0.0, 1.0);

        // Specular
        R = normalize(-reflect(lightDir, bumpNormal));
        light += pow(max(dot(R, V), 0.0), 0.25*50);
    }
    
    fragColor = textureColour * light; //vec4(norm.x * 0.5 + 0.5, norm.z * 0.5 + 0.5, 0, 1);
} 
