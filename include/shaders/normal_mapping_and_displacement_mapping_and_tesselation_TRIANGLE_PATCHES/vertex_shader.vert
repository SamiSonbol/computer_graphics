#version 440 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aBitangent;
layout(location = 4) in vec2 aTexture_coordinates;
layout(location = 5) in vec3 aColor;

out vec3 vPosition;
out vec3 vNormal;
out vec3 vColor;
out vec2 vTexture_coordinates;
out vec3 vTangent;
out vec3 vBitangent;

void main() {

    vPosition = aPosition;
    vNormal = aNormal;
    vTangent = aTangent;
    vBitangent = aBitangent;
    vTexture_coordinates = aTexture_coordinates;
    vColor = aColor;

    gl_Position = vec4(vPosition, 1.0);
    
};
